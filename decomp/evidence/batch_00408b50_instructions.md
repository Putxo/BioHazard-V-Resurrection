# Exact-attachment instruction evidence (14 bodies; padding excluded)

SHA-256: 323d1aabccc74505745588097e2b298b14e114393bfc24f6830e98b658e67815

## FUN_00408B50
Range 0x00408B50..0x00408B8D (exclusive); 61 bytes; SHA-256 36ba8dd4a216dc438433a31ac671ec00c39ca54728c9141fd41a5d0cd2031fae

```asm
  408b50:	56                                              	push   esi
  408b51:	8b f1                                           	mov    esi,ecx
  408b53:	8b 46 34                                        	mov    eax,DWORD PTR [esi+0x34]
  408b56:	85 c0                                           	test   eax,eax
  408b58:	7c 0d                                           	jl     0x408b67
  408b5a:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408b60:	56                                              	push   esi
  408b61:	50                                              	push   eax
  408b62:	e8 39 89 0a 00                                  	call   0x4b14a0
  408b67:	8b 0d b0 bb 65 01                               	mov    ecx,DWORD PTR ds:0x165bbb0
  408b6d:	e8 9e e3 0d 00                                  	call   0x4e6f10
  408b72:	f7 d8                                           	neg    eax
  408b74:	1b c0                                           	sbb    eax,eax
  408b76:	05 9e 00 00 00                                  	add    eax,0x9e
  408b7b:	56                                              	push   esi
  408b7c:	89 46 34                                        	mov    DWORD PTR [esi+0x34],eax
  408b7f:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408b85:	50                                              	push   eax
  408b86:	e8 35 e3 0a 00                                  	call   0x4b6ec0
  408b8b:	5e                                              	pop    esi
  408b8c:	c3                                              	ret
```

## FUN_00408B90
Range 0x00408B90..0x00408BBF (exclusive); 47 bytes; SHA-256 b32abdadd122586fbd5659def11c2f9f98d858e6beb54e31d4ea5cff0f712271

```asm
  408b90:	56                                              	push   esi
  408b91:	8b f1                                           	mov    esi,ecx
  408b93:	8b 46 34                                        	mov    eax,DWORD PTR [esi+0x34]
  408b96:	85 c0                                           	test   eax,eax
  408b98:	7c 1d                                           	jl     0x408bb7
  408b9a:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408ba0:	56                                              	push   esi
  408ba1:	50                                              	push   eax
  408ba2:	e8 c9 88 0a 00                                  	call   0x4b1470
  408ba7:	83 f8 02                                        	cmp    eax,0x2
  408baa:	74 0b                                           	je     0x408bb7
  408bac:	85 c0                                           	test   eax,eax
  408bae:	75 0b                                           	jne    0x408bbb
  408bb0:	c7 46 34 ff ff ff ff                            	mov    DWORD PTR [esi+0x34],0xffffffff
  408bb7:	b0 01                                           	mov    al,0x1
  408bb9:	5e                                              	pop    esi
  408bba:	c3                                              	ret
  408bbb:	32 c0                                           	xor    al,al
  408bbd:	5e                                              	pop    esi
  408bbe:	c3                                              	ret
```

## FUN_00408BC0
Range 0x00408BC0..0x00408BD5 (exclusive); 21 bytes; SHA-256 49cbde92e03b802650c614cad31f97259c993136cd9765d3b057173404f5059d

```asm
  408bc0:	8b 41 34                                        	mov    eax,DWORD PTR [ecx+0x34]
  408bc3:	85 c0                                           	test   eax,eax
  408bc5:	7c 0d                                           	jl     0x408bd4
  408bc7:	51                                              	push   ecx
  408bc8:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408bce:	50                                              	push   eax
  408bcf:	e8 5c 88 0a 00                                  	call   0x4b1430
  408bd4:	c3                                              	ret
```

## FUN_00408BE0
Range 0x00408BE0..0x00408C10 (exclusive); 48 bytes; SHA-256 d30eb0d1eec047fd9c4cfbda31d78f0e867e790e17d2752d821f1bec3d646e89

```asm
  408be0:	56                                              	push   esi
  408be1:	8b f1                                           	mov    esi,ecx
  408be3:	8b 46 34                                        	mov    eax,DWORD PTR [esi+0x34]
  408be6:	85 c0                                           	test   eax,eax
  408be8:	7c 24                                           	jl     0x408c0e
  408bea:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408bf0:	56                                              	push   esi
  408bf1:	50                                              	push   eax
  408bf2:	e8 39 88 0a 00                                  	call   0x4b1430
  408bf7:	8b 46 34                                        	mov    eax,DWORD PTR [esi+0x34]
  408bfa:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408c00:	56                                              	push   esi
  408c01:	50                                              	push   eax
  408c02:	e8 99 88 0a 00                                  	call   0x4b14a0
  408c07:	c7 46 34 ff ff ff ff                            	mov    DWORD PTR [esi+0x34],0xffffffff
  408c0e:	5e                                              	pop    esi
  408c0f:	c3                                              	ret
```

## FUN_00408C10
Range 0x00408C10..0x00408C22 (exclusive); 18 bytes; SHA-256 05e4a3217867fe821480643add4df0c5a5bdbd5b85f1803b8dfa66f4f0f39e69

```asm
  408c10:	51                                              	push   ecx
  408c11:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408c17:	68 98 00 00 00                                  	push   0x98
  408c1c:	e8 9f e2 0a 00                                  	call   0x4b6ec0
  408c21:	c3                                              	ret
```

## FUN_00408C30
Range 0x00408C30..0x00408C42 (exclusive); 18 bytes; SHA-256 0de9d8d8aab5353912b5d51b1f9be4581bf31d802910b15177e59b447777b6c4

```asm
  408c30:	51                                              	push   ecx
  408c31:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408c37:	68 99 00 00 00                                  	push   0x99
  408c3c:	e8 7f e2 0a 00                                  	call   0x4b6ec0
  408c41:	c3                                              	ret
```

## FUN_00408C50
Range 0x00408C50..0x00408C62 (exclusive); 18 bytes; SHA-256 2fdeca96d0f1b3ea3e45d7a1cfb1694af5dc47fda79b2f5897e3f22c7d5447a5

```asm
  408c50:	51                                              	push   ecx
  408c51:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408c57:	68 a2 00 00 00                                  	push   0xa2
  408c5c:	e8 5f e2 0a 00                                  	call   0x4b6ec0
  408c61:	c3                                              	ret
```

## FUN_00408C70
Range 0x00408C70..0x00408C9E (exclusive); 46 bytes; SHA-256 40bd535bf93abca763c5d449283110bd3ea4c3a19234148fe028699b433623ed

```asm
  408c70:	a1 60 16 6e 01                                  	mov    eax,ds:0x16e1660
  408c75:	8b 90 ec e4 01 00                               	mov    edx,DWORD PTR [eax+0x1e4ec]
  408c7b:	83 7a 58 02                                     	cmp    DWORD PTR [edx+0x58],0x2
  408c7f:	51                                              	push   ecx
  408c80:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408c86:	75 0b                                           	jne    0x408c93
  408c88:	68 a0 00 00 00                                  	push   0xa0
  408c8d:	e8 2e e2 0a 00                                  	call   0x4b6ec0
  408c92:	c3                                              	ret
  408c93:	68 d1 00 00 00                                  	push   0xd1
  408c98:	e8 23 e2 0a 00                                  	call   0x4b6ec0
  408c9d:	c3                                              	ret
```

## FUN_00408CA0
Range 0x00408CA0..0x00408CCE (exclusive); 46 bytes; SHA-256 7256d0e43b3448dd273bbf11643584a0d340b0c71a49288312fd1bc4216d055a

```asm
  408ca0:	a1 60 16 6e 01                                  	mov    eax,ds:0x16e1660
  408ca5:	8b 90 ec e4 01 00                               	mov    edx,DWORD PTR [eax+0x1e4ec]
  408cab:	83 7a 58 03                                     	cmp    DWORD PTR [edx+0x58],0x3
  408caf:	51                                              	push   ecx
  408cb0:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408cb6:	75 0b                                           	jne    0x408cc3
  408cb8:	68 cd 00 00 00                                  	push   0xcd
  408cbd:	e8 fe e1 0a 00                                  	call   0x4b6ec0
  408cc2:	c3                                              	ret
  408cc3:	68 cf 00 00 00                                  	push   0xcf
  408cc8:	e8 f3 e1 0a 00                                  	call   0x4b6ec0
  408ccd:	c3                                              	ret
```

## FUN_00408CD0
Range 0x00408CD0..0x00408CE9 (exclusive); 25 bytes; SHA-256 a3af90b04844ad349befcfb723c108152e328339cfa383dca54c681a2a2340fc

```asm
  408cd0:	33 c0                                           	xor    eax,eax
  408cd2:	38 44 24 04                                     	cmp    BYTE PTR [esp+0x4],al
  408cd6:	51                                              	push   ecx
  408cd7:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408cdd:	0f 94 c0                                        	sete   al
  408ce0:	50                                              	push   eax
  408ce1:	e8 da e1 0a 00                                  	call   0x4b6ec0
  408ce6:	c2 04 00                                        	ret    0x4
```

## FUN_00408CF0
Range 0x00408CF0..0x00408DCF (exclusive); 223 bytes; SHA-256 0f17ef30f83276ffb86a91cc9b6679b8e9b0724384856fe2a503f7b4dc68a75f

```asm
  408cf0:	56                                              	push   esi
  408cf1:	8b 74 24 08                                     	mov    esi,DWORD PTR [esp+0x8]
  408cf5:	85 f6                                           	test   esi,esi
  408cf7:	74 05                                           	je     0x408cfe
  408cf9:	be 01 00 00 00                                  	mov    esi,0x1
  408cfe:	8b 0d 3c 1d 6e 01                               	mov    ecx,DWORD PTR ds:0x16e1d3c
  408d04:	57                                              	push   edi
  408d05:	c1 e6 04                                        	shl    esi,0x4
  408d08:	8b 86 44 84 56 01                               	mov    eax,DWORD PTR [esi+0x1568444]
  408d0e:	6a 01                                           	push   0x1
  408d10:	50                                              	push   eax
  408d11:	68 b8 2d 6e 01                                  	push   0x16e2db8
  408d16:	e8 f5 be 90 00                                  	call   0xd14c10
  408d1b:	8b f8                                           	mov    edi,eax
  408d1d:	85 ff                                           	test   edi,edi
  408d1f:	74 13                                           	je     0x408d34
  408d21:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408d27:	57                                              	push   edi
  408d28:	e8 f3 6f 0a 00                                  	call   0x4afd20
  408d2d:	8b cf                                           	mov    ecx,edi
  408d2f:	e8 2c c3 91 00                                  	call   0xd25060
  408d34:	8b 8e 48 84 56 01                               	mov    ecx,DWORD PTR [esi+0x1568448]
  408d3a:	6a 01                                           	push   0x1
  408d3c:	51                                              	push   ecx
  408d3d:	8b 0d 3c 1d 6e 01                               	mov    ecx,DWORD PTR ds:0x16e1d3c
  408d43:	68 88 36 69 01                                  	push   0x1693688
  408d48:	e8 c3 be 90 00                                  	call   0xd14c10
  408d4d:	8b f8                                           	mov    edi,eax
  408d4f:	85 ff                                           	test   edi,edi
  408d51:	74 13                                           	je     0x408d66
  408d53:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408d59:	57                                              	push   edi
  408d5a:	e8 e1 6f 0a 00                                  	call   0x4afd40
  408d5f:	8b cf                                           	mov    ecx,edi
  408d61:	e8 fa c2 91 00                                  	call   0xd25060
  408d66:	8b 96 4c 84 56 01                               	mov    edx,DWORD PTR [esi+0x156844c]
  408d6c:	8b 0d 3c 1d 6e 01                               	mov    ecx,DWORD PTR ds:0x16e1d3c
  408d72:	6a 01                                           	push   0x1
  408d74:	52                                              	push   edx
  408d75:	68 30 28 6e 01                                  	push   0x16e2830
  408d7a:	e8 91 be 90 00                                  	call   0xd14c10
  408d7f:	8b f8                                           	mov    edi,eax
  408d81:	85 ff                                           	test   edi,edi
  408d83:	74 13                                           	je     0x408d98
  408d85:	8b 0d f0 a1 65 01                               	mov    ecx,DWORD PTR ds:0x165a1f0
  408d8b:	57                                              	push   edi
  408d8c:	e8 5f 86 0a 00                                  	call   0x4b13f0
  408d91:	8b cf                                           	mov    ecx,edi
  408d93:	e8 c8 c2 91 00                                  	call   0xd25060
  408d98:	8b 86 50 84 56 01                               	mov    eax,DWORD PTR [esi+0x1568450]
  408d9e:	8b 0d 3c 1d 6e 01                               	mov    ecx,DWORD PTR ds:0x16e1d3c
  408da4:	6a 01                                           	push   0x1
  408da6:	50                                              	push   eax
  408da7:	68 b0 22 6e 01                                  	push   0x16e22b0
  408dac:	e8 5f be 90 00                                  	call   0xd14c10
  408db1:	8b f0                                           	mov    esi,eax
  408db3:	85 f6                                           	test   esi,esi
  408db5:	5f                                              	pop    edi
  408db6:	74 13                                           	je     0x408dcb
  408db8:	8b 0d fc 2e 6e 01                               	mov    ecx,DWORD PTR ds:0x16e2efc
  408dbe:	56                                              	push   esi
  408dbf:	e8 9c c2 0b 00                                  	call   0x4c5060
  408dc4:	8b ce                                           	mov    ecx,esi
  408dc6:	e8 95 c2 91 00                                  	call   0xd25060
  408dcb:	5e                                              	pop    esi
  408dcc:	c2 04 00                                        	ret    0x4
```

## FUN_00408DD0
Range 0x00408DD0..0x00408DD8 (exclusive); 8 bytes; SHA-256 9831a54b80ef86314261f12d21d81e36c27f26c2012aa227e79a7efd133b557b

```asm
  408dd0:	6a 00                                           	push   0x0
  408dd2:	e8 19 ff ff ff                                  	call   0x408cf0
  408dd7:	c3                                              	ret
```

## FUN_00408DE0
Range 0x00408DE0..0x00408DFA (exclusive); 26 bytes; SHA-256 975600dec5e828f988d0653e86357bbd2411d1baeb9e393787424c01a2d388e0

```asm
  408de0:	56                                              	push   esi
  408de1:	8b f1                                           	mov    esi,ecx
  408de3:	e8 38 9c 8f 00                                  	call   0xd02a20
  408de8:	33 c0                                           	xor    eax,eax
  408dea:	89 46 28                                        	mov    DWORD PTR [esi+0x28],eax
  408ded:	89 46 30                                        	mov    DWORD PTR [esi+0x30],eax
  408df0:	c7 06 a8 e4 37 01                               	mov    DWORD PTR [esi],0x137e4a8
  408df6:	8b c6                                           	mov    eax,esi
  408df8:	5e                                              	pop    esi
  408df9:	c3                                              	ret
```

## FUN_00408E00
Range 0x00408E00..0x00408E1B (exclusive); 27 bytes; SHA-256 de13a469e2b684d3750f38914e4bdcaa1d6bba414f187806fd09b3c64fda0401

```asm
  408e00:	56                                              	push   esi
  408e01:	8b f1                                           	mov    esi,ecx
  408e03:	8d 46 08                                        	lea    eax,[esi+0x8]
  408e06:	50                                              	push   eax
  408e07:	c7 06 90 a9 37 01                               	mov    DWORD PTR [esi],0x137a990
  408e0d:	ff 15 9c 11 37 01                               	call   DWORD PTR ds:0x137119c
  408e13:	c7 06 7c a9 37 01                               	mov    DWORD PTR [esi],0x137a97c
  408e19:	5e                                              	pop    esi
  408e1a:	c3                                              	ret
```
