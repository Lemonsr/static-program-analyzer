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
    'Follows',
    'FollowsStar',
    'Modifies',
    'Parent',
    'ParentStar',
    'Uses'
]

autotester_path = '.\\Team18\\Code18\\Release\\AutoTester.exe'
test_path = '.\\Team18\\Tests18'

def find_if_fail():
    with open(f'{test_path}\\out.xml', 'r') as file:
        content = file.read()
        if content.find('failed') < 0:
            return False
    return True

def run_autotester(test_case_prefix):
    cmd = f'{autotester_path} {test_case_prefix}_source.txt {test_case_prefix}_queries.txt '
    cmd += f'{test_path}\\out.xml'
    status = subprocess.call(cmd, stdout=subprocess.DEVNULL)
    if status:
        print(f'Cant run autotester: {test_case_prefix}')
        quit()
    if find_if_fail():
        print(f'{test_case_prefix} failed\n')
    else:
        print(f'{test_case_prefix} passed\n')

run_autotester(f'{test_path}\\SimpleSelect\\entities')
run_autotester(f'{test_path}\\Pattern\\pattern')

for type in query_types:
    for case in test_cases:
        test_case_prefix = f'{test_path}\\{type}\\'
        test_case_prefix += type[0].lower()
        test_case_prefix += type[1:] + case
        run_autotester(test_case_prefix)
        
