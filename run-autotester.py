import subprocess

test_cases = [
    'IfinIfElse', 
    'IfinIfThen', 
    'IfinWhile', 
    'NoNesting', 
    'SingleIfElse',
    'SingleWhileLoop',
    'WhileInIfElse',
    'WhileInIfThen',
    'WhileinWhile'
]

query_types = [
    'Modifies',
    'ModifiesProc',
    'Uses',
    'UsesProc',
    'Follows',
    'FollowsStar',
    'Parent',
    'ParentStar',
    'Calls',
    'CallsStar',
]

invalid_query_types = [
    'General',
    'Modifies',
    'Uses',
    'Follows',
    'FollowsStar',
    'Parent',
    'ParentStar',
    'Calls',
    'CallsStar',
    'Pattern',
    'MultiClause'
]

pattern_types = [
    'Assign',
    'If',
    'While'
]

autotester_path = '.\\Team18\\Code18\\Release\\AutoTester.exe'
test_path = '.\\Team18\\Tests18'

def find_if_fail():
    with open(f'{test_path}\\out.xml', 'r') as file:
        content = file.read()
        hasFailed = not content.find('failed') < 0
        hasSyntaxError = not content.find('SyntaxError') < 0
        hasSemanticError = not content.find('SemanticError') < 0

        if hasFailed:
          print('There are failed test cases')
        if hasFailed and hasSyntaxError:
          print(f'There might be some unexpected syntax errors')
        if hasFailed and hasSemanticError:
          print(f'There might be some unexpected semantic errors')

    return True if hasFailed else False

def run_autotester(test_case_prefix):
    cmd = f'{autotester_path} {test_case_prefix}_source.txt {test_case_prefix}_queries.txt '
    cmd += f'{test_path}\\out.xml'
    status = subprocess.call(cmd, stdout=subprocess.DEVNULL)
    if status:
        print(f'Cant run autotester: {test_case_prefix}')
        quit()
    if find_if_fail():
        print(f'{test_case_prefix} failed\n')
        quit()
    else:
        print(f'{test_case_prefix} passed\n')

run_autotester(f'{test_path}\\SimpleSelect\\entities')

for pattern in pattern_types:
  run_autotester(f'{test_path}\\Pattern\\pattern{pattern}')

for type in query_types:
    for case in test_cases:
        test_case_prefix = f'{test_path}\\{type}\\'
        test_case_prefix += type[0].lower()
        test_case_prefix += type[1:] + case
        run_autotester(test_case_prefix)

run_autotester(f'{test_path}\\MultiClause\\multiclause')       

for invalid_type in invalid_query_types :
  if (invalid_type == 'Pattern'):
    for pattern in pattern_types: 
      run_autotester(f'{test_path}\\Invalid\{invalid_type}\invalid{invalid_type}{pattern}') 
  else:
    run_autotester(f'{test_path}\\Invalid\{invalid_type}\invalid{invalid_type}')     
