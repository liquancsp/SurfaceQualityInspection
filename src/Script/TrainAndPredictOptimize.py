import os
import time
import glob
import subprocess
import sys
import grid

def CreateNewDirectory(dir_name) : 
	if os.path.isdir(dir_name) != True :
		os.mkdir(dir_name)

def TrainDataType(select_file, alg_type) :
	train_file_name = select_file.replace('.select', '_' + alg_type + '.train')
	train_file_name = train_file_name.replace('/select/', '/train/')
	print 'train_file_name : ', train_file_name
	CreateNewDirectory('train')
	return train_file_name

#This function try to create .train data for a .test file and its .select file
def GenerateTrainingData(test_file_path, select_file, model_type, alg_type) :
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
	train_file_name = TrainDataType(select_file, alg_type)	
	train_file = file(train_file_name, 'w')
	for select_idx in select_list :
		feature = feature_list[int(select_idx)]
		#replace the feature type as model_type
		feature = feature.replace('-1 ', str(model_type) + ' ', 1)
		train_file.write(feature)
	train_file.close()

#Merge all seperate .train file into a whole train data. This file will be used to svm-train
def MergeTrainingData(alg_type) :
	CreateNewDirectory('traindata')
	train_model_type = './traindata/' + alg_type + '_train_data' 
	train_model_file = file(train_model_type, 'w')
	train_model_list = []
	
	train_file_type = './train/*_' + alg_type + '.train'
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

def MergeAllTrainingData() :
    train_file_type = './traindata/*_train_data'
    train_file_list = []
    for filename in glob.glob(train_file_type) :
        if filename.find('all_') != -1 :
            continue
        train_file_list.append(filename)
    train_model_type = './traindata/all_train_data';
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

def IsModify(test_file_list) :
    modify_list = []
    file_path = './modify.time'
    str = ''
    for filename in test_file_list :
        mtime = time.ctime(os.path.getmtime(filename))
        ctime = time.ctime(os.path.getctime(filename))
        str += mtime + ctime
    modify_list.append(str)
    if os.path.isfile(file_path) != True:
        modify_file = file(file_path, 'w+')
        modify_file.writelines(modify_list)
        modify_file.close()
        return True
    in_modify_file = file(file_path, 'r').readlines()
    file(file_path, 'w').writelines(modify_list)
    return (cmp(in_modify_file, modify_list) != 0)

def ParameterOptimization(alg_type_list, test_file_list) :
    CreateNewDirectory('train_param')
    train_param_list = []
    #if the test data are modified. And re-compute the best parameters.
    if IsModify(test_file_list) == True:
        options = ['-svmtrain', '/usr/bin/svm-train', '-gnuplot', '/usr/bin/gnuplot', '-log2c', '-5,5,1',
                   '-log2g', '-4,0,1', '-log2p', '-5,5,1', '-s', '3', '-t', '2', '-v', '5', '-m', '300']
        for type in alg_type_list :
            filename = './traindata/' + type + '_train_data'
            ret_best_params = grid.find_parameters(filename, options)
            print 'ret_best_param : ', ret_best_params
            tmp = str(ret_best_params[1].values()).strip('[] ')
            w_file = file('train_param/' + type + 'train_param', 'w').write(tmp)

            train_param_list.append(tmp.split(', '))
    else :
        for i in range(len(alg_type_list)) :
            tmp = str(file('train_param/' + alg_type_list[i] + 'train_param', 'r').readline())
            train_param_list.append(tmp.split(', '))
    print 'all train_param_list : ', train_param_list
    return train_param_list

test_file_list = []
scale_test_file_list = []
scale_parameter_name = []
alg_type_list = []
#search all .test file
for filename in glob.glob('*.test'):
	global alg_type
	last_slide = filename.rfind('_')
	last_dot = filename.rfind('.')
	alg_type_list.append(filename[last_slide + 1 : last_dot])
	test_file_list.append(filename)
print 'alg type list have : ', alg_type_list
print 'test_file_list : ' , test_file_list
if sys.argv[1] == '1' :
#do the svm-scale operation for each train data
    CreateNewDirectory('scale_test')
    CreateNewDirectory('scale_parameter')
    for i in range(len(test_file_list)) :
        scale_name = './scale_test/' + 'scale_' + test_file_list[i]
        scale_test_file_list.append(scale_name)
        scale_param = './scale_parameter/' + alg_type_list[i] + '.scalar'
        scale_parameter_name.append(scale_param)
        call_parameter = 'svm-scale -s ' + scale_param + ' ' + test_file_list[i] + '>' + scale_name
        retcode = subprocess.call(call_parameter, shell = True)
else :
    scale_test_file_list = test_file_list
print 'scale_test_file_list : ' , scale_test_file_list

#obtain all the .select file
select_file_list = []
for filename in glob.glob('./select/*.select'):
	select_file_list.append(filename)
select_file_list.sort()

print select_file_list
#Create training data for all .test file
model_type = 1
alg_type_flag = 0
for test_file in scale_test_file_list :
	model_type = 1
	for select_file in select_file_list :
		GenerateTrainingData(test_file, select_file, model_type, alg_type_list[alg_type_flag])
		model_type = model_type + 1
	alg_type_flag = alg_type_flag + 1;
#merge same *.test training data
for type_name in alg_type_list :
	MergeTrainingData(type_name)

MergeAllTrainingData()

CreateNewDirectory('model')
#svm model file output path

alg_type_list.append('all')

train_param_list = []
if sys.argv[2] == '1' :
#Calculate the best parameter for svm-train
    train_param_list = ParameterOptimization(alg_type_list, test_file_list)

for i in range(len(alg_type_list)) :
    # svm_type = sys.argv[1]
    # kernel_type = sys.argv[2]
    model_name = './model/' + alg_type_list[i] + '_train_data.model'
    filename = './traindata/' + alg_type_list[i] + '_train_data'
    if sys.argv[2] == '1' :
        parameter = 'svm-train ' + filename + ' ' + model_name + ' -s 3 -t 2 -p ' + train_param_list[i][0] + ' -c ' + \
                    train_param_list[i][1] + ' -g ' + train_param_list[i][2]
    else :
        parameter = 'svm-train ' + filename + ' ' + model_name + ' -s 0 -t 2'
    print 'parameter is : ' , parameter
    retcode = subprocess.call(parameter, shell = True) #training the model
#predict the new data by using the model

CreateNewDirectory('predict')
#predict all test file by using themselves model
alg_type_flag = 0
for filename in scale_test_file_list:
	type_name = alg_type_list[alg_type_flag]
	alg_type_flag = alg_type_flag + 1
	pred_file = './predict/' + type_name + '.pred'
	model_name = './model/' + type_name + '_train_data.model'
	retcode = subprocess.call(["svm-predict", filename, model_name, pred_file])
#predoct all test file by using whole model
alg_type_flag = 0
for filename in scale_test_file_list :
    type_name = alg_type_list[alg_type_flag]
    alg_type_flag = alg_type_flag + 1
    pred_file = './predict/all_' + type_name + '.pred'
    model_name = './model/all_train_data.model'
    retcode = subprocess.call(["svm-predict", filename, model_name, pred_file])