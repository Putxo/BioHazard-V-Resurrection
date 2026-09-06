import pathlib, sys
ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.validate.validate import validate_functions, validate_claims


def test_empty_tables_are_valid():
    assert validate_functions([]) == []
    assert validate_claims([]) == []


def test_duplicate_function_identity_is_rejected():
    rows=[
        {'stable_id':'FUN_00401000','va':'0x00401000','status':'DISCOVERED','size':'16','evidence':''},
        {'stable_id':'FUN_00401000','va':'0x00402000','status':'DISCOVERED','size':'16','evidence':''},
    ]
    assert any('duplicate stable_id' in e for e in validate_functions(rows))


def test_verified_requires_evidence():
    rows=[{'stable_id':'FUN_00401000','va':'0x00401000','status':'VERIFIED','size':'','evidence':''}]
    assert any('requires evidence' in e for e in validate_functions(rows))


def test_duplicate_active_claim_is_rejected():
    rows=[
        {'claim_id':'a','stable_id':'FUN_00401000','active':'true'},
        {'claim_id':'b','stable_id':'FUN_00401000','active':'true'},
    ]
    assert any('duplicate active claim' in e for e in validate_claims(rows))
