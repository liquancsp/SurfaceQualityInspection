import os
import time
import glob
import subprocess
import sys

test_file_list = []
alg_type_list = []
#search all .test file

for filename in glob.glob('*.test'):
	global alg_type
	last_slide = filename.rfind('_')
	last_dot = filename.rfind('.')
	alg_type_list.append(filename[last_slide + 1 : last_dot])
	test_file_list.append(filename)

for filename in test_file_list:
	last_dot = filename.rfind('.')
	test_name = filename[0 : last_dot]
	type_name = alg_type_list[alg_type_flag]
	alg_type_flag = alg_type_flag + 1
	pred_file = './predict/' + test_name + '.pred'
	model_name = './model/' + type_name + '_train_data.model'
	retcode = subprocess.call(["svm-predict", filename, model_name, pred_file])
