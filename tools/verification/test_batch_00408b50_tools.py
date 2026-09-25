#!/usr/bin/env python3
"""Source-only checks. No proprietary input is needed or downloaded by CI."""
import hashlib,json,pathlib,unittest
from verify_batch_00408b50 import CPU,Fixture,pe_sections
ROOT=pathlib.Path(__file__).resolve().parents[2]
class Tests(unittest.TestCase):
    def test_source_identity(self):
        manifest=json.loads((ROOT/'decomp/evidence/batch_00408b50.json').read_text())
        for name,digest in manifest['source_sha256'].items():
            self.assertEqual(hashlib.sha256((ROOT/name).read_bytes()).hexdigest(),digest,name)
        self.assertEqual(len(manifest['entries']),14)
        self.assertEqual(sum(x['size'] for x in manifest['entries']),632)
    def test_wrong_target_rejected_before_decoding(self):
        with self.assertRaises(ValueError): pe_sections(b'MZ'+b'\0'*1022)
        with self.assertRaises(ValueError): pe_sections(b'')
    def test_target_memory_is_not_fabricated(self):
        fixture=Fixture()
        with self.assertRaises(ValueError): fixture.read(0xDEADBEEF)
        with self.assertRaises(ValueError): fixture.write(0xDEADBEEF,0)
        with self.assertRaises(ValueError): fixture.call(0xBAD,0,[])
    def test_arithmetic_flags(self):
        cpu=CPU(Fixture(),{},set())
        cpu.arithmetic(0,0,0,32,True)
        self.assertTrue(cpu.z); self.assertFalse(cpu.cf)
        cpu.arithmetic(0,1,-1,32,True)
        self.assertTrue(cpu.cf); self.assertTrue(cpu.sf); self.assertFalse(cpu.z)
        cpu.arithmetic(0,0x80000000,-0x80000000,32,True)
        self.assertTrue(cpu.cf); self.assertTrue(cpu.sf); self.assertTrue(cpu.of)
        cpu.arithmetic(0xFFFFFFFF,0x100000000,-1,32,True)
        self.assertTrue(cpu.cf); self.assertTrue(cpu.sf)
        cpu.arithmetic(0xFFFFFFFF,0x9E,0x10000009D,32,False)
        self.assertTrue(cpu.cf); self.assertFalse(cpu.sf)
    def test_partial_register_write(self):
        cpu=CPU(Fixture(),{},set()); cpu.r['eax']=0xAABBCCDD
        cpu.put('al',1)
        self.assertEqual(cpu.r['eax'],0xAABBCC01)
    def test_unsupported_instruction_fails_closed(self):
        cpu=CPU(Fixture(),{0x408B50:(1,'unsupported','')},set())
        with self.assertRaises(ValueError): cpu.run(0x408B50,0)
if __name__=='__main__': unittest.main()
