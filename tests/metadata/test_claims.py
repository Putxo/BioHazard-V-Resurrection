import pathlib, sys
ROOT=pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.claims.claim import can_claim


def test_unclaimed_function_can_be_claimed():
    assert can_claim([], 'FUN_00401000')


def test_active_claim_blocks_second_owner():
    rows=[{'stable_id':'FUN_00401000','active':'true'}]
    assert not can_claim(rows, 'FUN_00401000')


def test_released_claim_does_not_block():
    rows=[{'stable_id':'FUN_00401000','active':'false'}]
    assert can_claim(rows, 'FUN_00401000')
