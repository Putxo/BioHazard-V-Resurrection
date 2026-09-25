# Synthetic CSV fixtures test parsing only; they are not canonical membership evidence.
from pathlib import Path
import sys,tempfile
sys.path.insert(0,str(Path(__file__).resolve().parents[1]/'tools'))
from audit_membership import read_candidates
checks=0
with tempfile.TemporaryDirectory() as tmp:
    p=Path(tmp)/'fixture.csv'
    p.write_text('rva,va,confidence,reason\n34560,4228864,1,test\n35568,4229872,1,test\n')
    assert read_candidates(p,2)=={4228864,4229872};checks+=1
    for text,expected in [
        ('34560,0x00408700,1,test\n',1),
        ('34560,4228865,1,test\n',1),
        ('34560,4228864,1,test\n34560,4228864,1,test\n',2),
        ('34560,4228864,1,test\n',2)]:
        p.write_text(text)
        try: read_candidates(p,expected)
        except ValueError: checks+=1
        else: raise AssertionError('Invalid candidate evidence accepted')
print(f'Synthetic membership-parser tests PASS: {checks}/5; no canonical audit claimed')
