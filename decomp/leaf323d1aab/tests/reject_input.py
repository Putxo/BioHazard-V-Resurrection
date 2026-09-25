from pathlib import Path
import sys,tempfile
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from recover_leaves import read_target, SIZE
checks=0
with tempfile.TemporaryDirectory() as temp:
    p=Path(temp)/'wrong-input.dat'
    for payload in [b'',b'MZ',bytes(1024),bytes(SIZE)]:
        p.write_bytes(payload)
        try: read_target(p)
        except ValueError: checks+=1
        else: raise AssertionError('Wrong input was accepted')
    try: read_target(Path(temp)/'missing.exe')
    except FileNotFoundError: checks+=1
    else: raise AssertionError('Missing input was accepted')
print(f'Wrong/missing input rejection PASS: {checks}/5')
