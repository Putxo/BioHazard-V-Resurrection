import pathlib, sys
ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.common.database import function_rows, active_claims


def test_canonical_function_table_parses_and_has_unique_addresses():
    rows = function_rows()
    assert rows
    ids = [r['stable_id'] for r in rows]
    vas = [r['va'] for r in rows]
    assert len(ids) == len(set(ids))
    assert len(vas) == len(set(vas))
    for row in rows:
        assert row['stable_id'].startswith('FUN_')
        assert int(row['va'], 0) >= 0x00401000
        assert int(row['rva'], 0) == int(row['va'], 0) - 0x00400000
        assert int(row['size'], 0) > 0


def test_pilot_claims_resolve_to_canonical_functions():
    rows = {r['stable_id'] for r in function_rows()}
    claims = active_claims()
    assert claims
    assert all(c['stable_id'] in rows for c in claims)
