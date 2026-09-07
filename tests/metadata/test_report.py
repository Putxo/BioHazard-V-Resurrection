import pathlib
import sys

ROOT = pathlib.Path(__file__).resolve().parents[2]
sys.path.insert(0, str(ROOT))

from tools.reports.report import replace_marked


def test_missing_markers_do_not_publish_branch_progress():
    text = '# Integration\n\nDashboard is main-only.\n'
    assert replace_marked(text, '<!-- start -->', '<!-- end -->', 'new') == text


def test_existing_markers_are_updated():
    text = 'before\n<!-- start -->\nold\n<!-- end -->\nafter\n'
    expected = 'before\n<!-- start -->\nnew\n<!-- end -->\nafter\n'
    assert replace_marked(text, '<!-- start -->', '<!-- end -->', 'new') == expected
