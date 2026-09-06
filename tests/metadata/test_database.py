import pathlib, sys
ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))
from tools.common.database import function_rows, active_claims


def test_empty_canonical_tables_parse():
    assert function_rows() == []
    assert active_claims() == []
