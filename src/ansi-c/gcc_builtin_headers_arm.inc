"// clang-format off\n"
"int __builtin_arm_getwcx(int);\n"
"void __builtin_arm_setwcx(int, int);\n"
"int __builtin_arm_textrmsb(__gcc_v8qi, int);\n"
"int __builtin_arm_textrmsh(__gcc_v4hi, int);\n"
"int __builtin_arm_textrmsw(__gcc_v2si, int);\n"
"int __builtin_arm_textrmub(__gcc_v8qi, int);\n"
"int __builtin_arm_textrmuh(__gcc_v4hi, int);\n"
"int __builtin_arm_textrmuw(__gcc_v2si, int);\n"
"__gcc_v8qi __builtin_arm_tinsrb(__gcc_v8qi, int);\n"
"__gcc_v4hi __builtin_arm_tinsrh(__gcc_v4hi, int);\n"
"__gcc_v2si __builtin_arm_tinsrw(__gcc_v2si, int);\n"
"long long __builtin_arm_tmia(long long, int, int);\n"
"long long __builtin_arm_tmiabb(long long, int, int);\n"
"long long __builtin_arm_tmiabt(long long, int, int);\n"
"long long __builtin_arm_tmiaph(long long, int, int);\n"
"long long __builtin_arm_tmiatb(long long, int, int);\n"
"long long __builtin_arm_tmiatt(long long, int, int);\n"
"int __builtin_arm_tmovmskb(__gcc_v8qi);\n"
"int __builtin_arm_tmovmskh(__gcc_v4hi);\n"
"int __builtin_arm_tmovmskw(__gcc_v2si);\n"
"long long __builtin_arm_waccb(__gcc_v8qi);\n"
"long long __builtin_arm_wacch(__gcc_v4hi);\n"
"long long __builtin_arm_waccw(__gcc_v2si);\n"
"__gcc_v8qi __builtin_arm_waddb(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v8qi __builtin_arm_waddbss(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v8qi __builtin_arm_waddbus(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_waddh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_waddhss(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_waddhus(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_waddw(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v2si __builtin_arm_waddwss(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v2si __builtin_arm_waddwus(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v8qi __builtin_arm_walign(__gcc_v8qi, __gcc_v8qi, int);\n"
"long long __builtin_arm_wand(long long, long long);\n"
"long long __builtin_arm_wandn(long long, long long);\n"
"__gcc_v8qi __builtin_arm_wavg2b(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v8qi __builtin_arm_wavg2br(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wavg2h(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_wavg2hr(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v8qi __builtin_arm_wcmpeqb(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wcmpeqh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wcmpeqw(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v8qi __builtin_arm_wcmpgtsb(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wcmpgtsh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wcmpgtsw(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v8qi __builtin_arm_wcmpgtub(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wcmpgtuh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wcmpgtuw(__gcc_v2si, __gcc_v2si);\n"
"long long __builtin_arm_wmacs(long long, __gcc_v4hi, __gcc_v4hi);\n"
"long long __builtin_arm_wmacsz(__gcc_v4hi, __gcc_v4hi);\n"
"long long __builtin_arm_wmacu(long long, __gcc_v4hi, __gcc_v4hi);\n"
"long long __builtin_arm_wmacuz(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_wmadds(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_wmaddu(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v8qi __builtin_arm_wmaxsb(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wmaxsh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wmaxsw(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v8qi __builtin_arm_wmaxub(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wmaxuh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wmaxuw(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v8qi __builtin_arm_wminsb(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wminsh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wminsw(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v8qi __builtin_arm_wminub(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wminuh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wminuw(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v4hi __builtin_arm_wmulsm(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_wmulul(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_wmulum(__gcc_v4hi, __gcc_v4hi);\n"
"long long __builtin_arm_wor(long long, long long);\n"
"__gcc_v2si __builtin_arm_wpackdss(long long, long long);\n"
"__gcc_v2si __builtin_arm_wpackdus(long long, long long);\n"
"__gcc_v8qi __builtin_arm_wpackhss(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v8qi __builtin_arm_wpackhus(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_wpackwss(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v4hi __builtin_arm_wpackwus(__gcc_v2si, __gcc_v2si);\n"
"long long __builtin_arm_wrord(long long, long long);\n"
"long long __builtin_arm_wrordi(long long, int);\n"
"__gcc_v4hi __builtin_arm_wrorh(__gcc_v4hi, long long);\n"
"__gcc_v4hi __builtin_arm_wrorhi(__gcc_v4hi, int);\n"
"__gcc_v2si __builtin_arm_wrorw(__gcc_v2si, long long);\n"
"__gcc_v2si __builtin_arm_wrorwi(__gcc_v2si, int);\n"
"__gcc_v2si __builtin_arm_wsadb(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v2si __builtin_arm_wsadbz(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v2si __builtin_arm_wsadh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wsadhz(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_wshufh(__gcc_v4hi, int);\n"
"long long __builtin_arm_wslld(long long, long long);\n"
"long long __builtin_arm_wslldi(long long, int);\n"
"__gcc_v4hi __builtin_arm_wsllh(__gcc_v4hi, long long);\n"
"__gcc_v4hi __builtin_arm_wsllhi(__gcc_v4hi, int);\n"
"__gcc_v2si __builtin_arm_wsllw(__gcc_v2si, long long);\n"
"__gcc_v2si __builtin_arm_wsllwi(__gcc_v2si, int);\n"
"long long __builtin_arm_wsrad(long long, long long);\n"
"long long __builtin_arm_wsradi(long long, int);\n"
"__gcc_v4hi __builtin_arm_wsrah(__gcc_v4hi, long long);\n"
"__gcc_v4hi __builtin_arm_wsrahi(__gcc_v4hi, int);\n"
"__gcc_v2si __builtin_arm_wsraw(__gcc_v2si, long long);\n"
"__gcc_v2si __builtin_arm_wsrawi(__gcc_v2si, int);\n"
"long long __builtin_arm_wsrld(long long, long long);\n"
"long long __builtin_arm_wsrldi(long long, int);\n"
"__gcc_v4hi __builtin_arm_wsrlh(__gcc_v4hi, long long);\n"
"__gcc_v4hi __builtin_arm_wsrlhi(__gcc_v4hi, int);\n"
"__gcc_v2si __builtin_arm_wsrlw(__gcc_v2si, long long);\n"
"__gcc_v2si __builtin_arm_wsrlwi(__gcc_v2si, int);\n"
"__gcc_v8qi __builtin_arm_wsubb(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v8qi __builtin_arm_wsubbss(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v8qi __builtin_arm_wsubbus(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wsubh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_wsubhss(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v4hi __builtin_arm_wsubhus(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wsubw(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v2si __builtin_arm_wsubwss(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v2si __builtin_arm_wsubwus(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v4hi __builtin_arm_wunpckehsb(__gcc_v8qi);\n"
"__gcc_v2si __builtin_arm_wunpckehsh(__gcc_v4hi);\n"
"long long __builtin_arm_wunpckehsw(__gcc_v2si);\n"
"__gcc_v4hi __builtin_arm_wunpckehub(__gcc_v8qi);\n"
"__gcc_v2si __builtin_arm_wunpckehuh(__gcc_v4hi);\n"
"long long __builtin_arm_wunpckehuw(__gcc_v2si);\n"
"__gcc_v4hi __builtin_arm_wunpckelsb(__gcc_v8qi);\n"
"__gcc_v2si __builtin_arm_wunpckelsh(__gcc_v4hi);\n"
"long long __builtin_arm_wunpckelsw(__gcc_v2si);\n"
"__gcc_v4hi __builtin_arm_wunpckelub(__gcc_v8qi);\n"
"__gcc_v2si __builtin_arm_wunpckeluh(__gcc_v4hi);\n"
"long long __builtin_arm_wunpckeluw(__gcc_v2si);\n"
"__gcc_v8qi __builtin_arm_wunpckihb(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wunpckihh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wunpckihw(__gcc_v2si, __gcc_v2si);\n"
"__gcc_v8qi __builtin_arm_wunpckilb(__gcc_v8qi, __gcc_v8qi);\n"
"__gcc_v4hi __builtin_arm_wunpckilh(__gcc_v4hi, __gcc_v4hi);\n"
"__gcc_v2si __builtin_arm_wunpckilw(__gcc_v2si, __gcc_v2si);\n"
"long long __builtin_arm_wxor(long long, long long);\n"
"long long __builtin_arm_wzero();\n"
"// clang-format on\n"
