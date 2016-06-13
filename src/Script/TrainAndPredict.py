import os
import time
import glob
import subprocess
import sys

def CreateNewDirectory(dir_name) : 
	if os.path.isdir(dir_name) != True :
		os.mkdir(dir_name)

def TrainDataType(select_file, alg_type, path) :
	train_file_name = select_file.replace('.select', '_' + alg_type + '.train')
	train_file_name = train_file_name.replace('/select/', '/train/')
	print 'train_file_name : ', train_file_name
	CreateNewDirectory(path + 'train')
	return train_file_name

#This function try to create .train data for a .test file and its .select file
def GenerateTrainingData(test_file_path, select_file, model_type, alg_type, path) :
	feature_list = []
	f = file(test_file_path)
	print 'test_file_path is : ' , test_file_path
	print 'model_type is : ', model_type
	while True:
		line = f.readline();
                if len(line) == 0 :
			break;
		feature_list.append(line)
	f.close();
	f_select = file(select_file)
	select_list = []
	while True:
		line = f_select.readline()
		if len(line) == 0 :
			break
		select_list.append(line)
	f_select.close()
	#Obtain training data path and name
	train_file_name = TrainDataType(select_file, alg_type, path)
	train_file = file(train_file_name, 'w')
	for select_idx in select_list :
		feature = feature_list[int(select_idx)]
		#replace the feature type as model_type
		feature = feature.replace('-1 ', str(model_type) + ' ', 1)
		train_file.write(feature)
	train_file.close()

#Merge all seperate .train file into a whole train data. This file will be used to svm-train
def MergeTrainingData(alg_type, path) :
	CreateNewDirectory(path + 'traindata')
	train_model_type = path + 'traindata/' + alg_type + '_train_data'
	train_model_file = file(train_model_type, 'w')
	train_model_list = []
	
	train_file_type = path + 'train/*_' + alg_type + '.train'
	print 'train file type is : ', train_file_type
	for filename in glob.glob(train_file_type) :
		train_model_list.append(filename)
	train_model_list.sort()
	print 'train_model_list : ', train_model_list
	for filename in train_model_list :
		f_train = file(filename)
		while True :
			line = f_train.readline()
			if len(line) == 0:
				break
			train_model_file.write(line)
		f_train.close()
	train_model_file.close()

def MergeAllTrainingData(path) :
    train_file_type = path + 'traindata/*_train_data'
    train_file_list = []
    for filename in glob.glob(train_file_type) :
        if filename.find('all_') != -1 :
            continue
        train_file_list.append(filename)
    train_model_type = path + 'traindata/all_train_data';
    train_model_file = file(train_model_type, 'w')
    for filename in train_file_list :
        print 'train data is : ', filename
        f_train = file(filename)
        while True :
            line = f_train.readline()
            if len(line) == 0 :
                break
            train_model_file.write(line)
        f_train.close()
    train_model_file.close()

def ImportTestFile(test_file_list, alg_type_list, select_file_list, path) :
    #search all .test file
    dir_path = path + '*.test'
    for filename in glob.glob(dir_path):
    	global alg_type
    	last_slide = filename.rfind('_')
    	last_dot = filename.rfind('.')
    	alg_type_list.append(filename[last_slide + 1 : last_dot])
    	test_file_list.append(filename)
    print 'alg type list have : ', alg_type_list
    #obtain all the .select file

    for filename in glob.glob(path + 'select/*.select'):
	    select_file_list.append(filename)
    select_file_list.sort()
    print test_file_list
    print select_file_list
    print '=================================================================='
    print 'Test file import completed!'
    print '=================================================================='
#test_file_list = []
#alg_type_list = []
#select_file_list = []

def CreateTrainingData(test_file_list, alg_type_list, select_file_list, path) :
    #Create training data for all .test file
    model_type = 1
    alg_type_flag = 0
    for test_file in test_file_list :
        model_type = 1
        for select_file in select_file_list :
		    GenerateTrainingData(test_file, select_file, model_type, alg_type_list[alg_type_flag], path)
		    model_type = model_type + 1
        alg_type_flag = alg_type_flag + 1
    for type_name in alg_type_list :
	    MergeTrainingData(type_name, path)
    MergeAllTrainingData(path)
    print '=================================================================='
    print 'Generate training data completed!'
    print '=================================================================='

def TrainingModel(test_file_list, alg_type_list, select_file_list, path) :
    CreateNewDirectory(path + 'model')
    #svm model file output path
    alg_type_list.append('all')
    for type_name in alg_type_list :
        # svm_type = sys.argv[1]
        # kernel_type = sys.argv[2]
        model_name = path + 'model/' + type_name + '_train_data.model'
        filename = path + 'traindata/' + type_name + '_train_data'
        # parameter = 'svm-train ' + filename + ' ' + model_name + ' -s ' + svm_type + ' -t ' + kernel_type
        parameter = 'svm-train ' + filename + ' ' + model_name + ' -s 0 -t 2'
        print 'parameter is : ' , parameter
        retcode = subprocess.call(parameter, shell = True) #training the model
    print '=================================================================='
    print 'Train model completed!'
    print '=================================================================='
    #predict the new data by using the model

def Predict(test_file_list, alg_type_list, select_file_list, path) :
    CreateNewDirectory(path + 'predict')
    #predict all test file by using themselves model
    alg_type_flag = 0
    for filename in test_file_list:
    	type_name = alg_type_list[alg_type_flag]
        alg_type_flag = alg_type_flag + 1
        pred_file = path + 'predict/' + type_name + '.pred'
        model_name = path + 'model/' + type_name + '_train_data.model'
        retcode = subprocess.call(["svm-predict", filename, model_name, pred_file])
    #predoct all test file by using whole model
    alg_type_flag = 0
    for filename in test_file_list :
        type_name = alg_type_list[alg_type_flag]
        alg_type_flag = alg_type_flag + 1
        pred_file = path + 'predict/all_' + type_name + '.pred'
        model_name = path + 'model/all_train_data.model'
        retcode = subprocess.call(["svm-predict", filename, model_name, pred_file])
    print '=================================================================='
    print 'Predict completed!'
    print '=================================================================='

def Process(path) :
    test_file_list = []
    alg_type_list = []
    select_file_list = []
    ImportTestFile(test_file_list, alg_type_list, select_file_list, path)
    CreateTrainingData(test_file_list, alg_type_list, select_file_list, path)
    TrainingModel(test_file_list, alg_type_list, select_file_list, path)
    Predict(test_file_list, alg_type_list, select_file_list, path)

if __name__ == "__main__":
    Process('./')