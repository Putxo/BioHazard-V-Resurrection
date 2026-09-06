import pathlib, sys
ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.validate.validate import validate_functions, validate_claims

BASE={'fast_pass':'false','fast_pass_validated':'false','converted':'false','refined':'false','verified':'false','matched':'false','linked':'false','evidence':''}

def record(**kwargs):
    r=dict(BASE); r.update({'stable_id':'FUN_00401000','va':'0x00401000','workflow_status':'DISCOVERED','size':'16'}); r.update(kwargs); return r

def test_empty_tables_are_valid():
    assert validate_functions([]) == []
    assert validate_claims([]) == []

def test_duplicate_function_identity_is_rejected():
    rs=[record(),record(va='0x00402000')]
    assert any('duplicate stable_id' in e for e in validate_functions(rs))

def test_verified_requires_evidence():
    rs=[record(verified='true')]
    assert any('require evidence' in e for e in validate_functions(rs))

def test_independent_tiers_are_allowed():
    rs=[record(converted='true',matched='false',evidence='')]
    assert validate_functions(rs)==[]

def test_duplicate_active_claim_is_rejected():
    rs=[{'claim_id':'a','stable_id':'FUN_00401000','active':'true'},{'claim_id':'b','stable_id':'FUN_00401000','active':'true'}]
    assert any('duplicate active claim' in e for e in validate_claims(rs))
