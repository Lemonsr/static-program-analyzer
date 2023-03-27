import subprocess
import sys
import os

ignored_list = ["Sample_queries.txt","Sample_source.txt","SystemTest"]
unique_txt_counter = {}
autotester_path = '.\\Team18\\Code18\\Release\\AutoTester.exe'
test_path = '.\\Team18\\Tests18'
out_path = '.\\Team18\\Tests18'

def find_if_fail():
    with open(f'{out_path}\\out.xml', 'r') as file:
        content = file.read()
        hasFailed = not content.find('failed') < 0
        hasSyntaxError = not content.find('SyntaxError') < 0
        hasSemanticError = not content.find('SemanticError') < 0
        hasExceptionErr = not content.find('exception') <0
        hasTimeOut = not content.find('timeout') <0

        if hasFailed:
          print('There are failed test cases')
        if hasFailed and hasSyntaxError:
          print(f'There might be some unexpected syntax errors')
        if hasFailed and hasSemanticError:
          print(f'There might be some unexpected semantic errors')
        if hasExceptionErr:
          print(f'There are some cases with exception errors')
        if hasTimeOut:
          print(f'There are some cases that exceeded the time limit!')

    return True if hasFailed or hasExceptionErr or hasTimeOut else False

def run_autotester(test_case_prefix):
    cmd = f'{autotester_path} {test_case_prefix}_source.txt {test_case_prefix}_queries.txt '
    cmd += f'{out_path}\\out.xml'
    status = subprocess.call(cmd, stdout=subprocess.DEVNULL)
    if status:
        print(f'Cant run autotester: {test_case_prefix}')
        sys.exit(1)
    if find_if_fail():
        print(f'{test_case_prefix} failed\n')
        sys.exit(1)
    else:
        print(f'{test_case_prefix} passed\n')

def get_file_names_and_location(location):
  for detected_item in os.listdir(location):
      if str(detected_item) in ignored_list:
          continue
      try:
          file_location = location + '\\' + str(detected_item)
          if os.path.isdir(file_location):
              get_file_names_and_location(file_location)
          if detected_item.endswith(".txt") and not detected_item.endswith("line.txt"):
              file_name = str(detected_item)
              file_absolute_name = file_name.replace('_source.txt', '').replace('_queries.txt', '')
              key = (location,file_absolute_name)
              counter = unique_txt_counter.get(key)
              if counter:
                  unique_txt_counter[key] = counter + 1
              else:
                  unique_txt_counter[key] = 1
      except Exception as e:
          print(location)
          print("No files found here!")

def start_test(dict_with_location_name):
    for key, value in dict_with_location_name.items():
        location = key[0]
        txtfileName = key[1]
        if value == 1:
            print("Only 1 file is detected at %s for %s" % (location, txtfileName))
        elif value == 2:
            run_autotester(location + '\\%s' % txtfileName)
        else:
            print("There is more than 2 files at %s for %s" % (location, txtfileName))

if len(sys.argv) == 3:
  run_autotester(test_path + "\\%s" % sys.argv[1] + '\\%s' % sys.argv[2])
  quit()
if len(sys.argv) == 2:
  test_path = test_path + "\\%s" % sys.argv[1]
else:
  test_path = '.\\Team18\\Tests18'

get_file_names_and_location(test_path)
start_test(unique_txt_counter)
