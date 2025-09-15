#include "isr.h"
#include "idt.h"
#include "../../utils/string.h"
#include "../../cliutils.h"


ISR_Handler g_ISRHandlers[256];
extern void switch_to_kernel();


static void usr_handler(ISR_StackFrame* regs){
    // a=1 ; print
    // a=80; exit
    if(regs->eax==1)
        cli_printf("%s", regs->ebx);
    else if(regs->eax==60){
        switch_to_kernel();
    }else
        cli_printf("Unknown syscall: %d \n", regs->eax);
}

static void default_ISR_handlers(){
    set_ISR_Handler(128, usr_handler);
}








// cant be static, we need to access it from asm
void isr_common_handler(ISR_StackFrame* frame) {
    int int_no = frame->int_no;

    if(int_no<32) {
        while(1);
        switch (int_no) {
            case 0: cli_printf("Division by zero error\n"); break;
            case 1: cli_printf("Debug exception\n"); break;
            case 2: cli_printf("Non-maskable interrupt\n"); break;
            case 3: cli_printf("Breakpoint\n"); break;
            case 4: cli_printf("Overflow\n"); break;
            case 5: cli_printf("Bounds check error\n"); break;
            case 6: cli_printf("Invalid opcode\n"); break;
            case 7: cli_printf("Device not available\n"); break;
            case 8: cli_printf("Double fault\n"); break;
            case 9: cli_printf("Coprocessor segment overrun\n"); break;
            case 10: cli_printf("Invalid TSS\n"); break;
            case 11: cli_printf("Segment not present\n"); break;
            case 12: cli_printf("Stack fault\n"); break;
            case 13: cli_printf("General protection fault\n"); break;
            case 14: cli_printf("Page fault\n"); break;
            case 15: cli_printf("Reserved\n"); break;
        }
        asm volatile("cli");
        asm volatile("hlt");
    }
    
    if(g_ISRHandlers[int_no]!=NULL){
        g_ISRHandlers[int_no](frame);
    }else{
        cli_error("Received unhandled interrupt: %d \n", int_no);
    }
}


/*
Note: these are only mirrors of asm functions, dont try to modify them from here
Dont implement logic int asm file either, we will add custom handlers using _set_ISR_Handler function
The only purpose of these functions is to fill up the idt 
and using isr_common_handler channel them from asm to C
*/
#pragma region ISR Mirrors Asm
void ISR_0();
void ISR_1();
void ISR_2();
void ISR_3();
void ISR_4();
void ISR_5();
void ISR_6();   
void ISR_7();
void ISR_8();
void ISR_9();
void ISR_10();
void ISR_11();
void ISR_12();
void ISR_13();
void ISR_14();
void ISR_15();
void ISR_16();
void ISR_17();
void ISR_18();
void ISR_19();
void ISR_20();  
void ISR_21();
void ISR_22();
void ISR_23();
void ISR_24();
void ISR_25();
void ISR_26();
void ISR_27();
void ISR_28();
void ISR_29();
void ISR_30();
void ISR_31();
void ISR_32();
void ISR_33();
void ISR_34();
void ISR_35();
void ISR_36();
void ISR_37();
void ISR_38();
void ISR_39();
void ISR_40();
void ISR_41();
void ISR_42();
void ISR_43();
void ISR_44();
void ISR_45();
void ISR_46();
void ISR_47();
void ISR_48();
void ISR_49();
void ISR_50();
void ISR_51();
void ISR_52();
void ISR_53();
void ISR_54();
void ISR_55();
void ISR_56();
void ISR_57();
void ISR_58();
void ISR_59();
void ISR_60();
void ISR_61();
void ISR_62();
void ISR_63();
void ISR_64();
void ISR_65();
void ISR_66();
void ISR_67();
void ISR_68();
void ISR_69();
void ISR_70();
void ISR_71();
void ISR_72();
void ISR_73();
void ISR_74();
void ISR_75();
void ISR_76();
void ISR_77();
void ISR_78();
void ISR_79();
void ISR_80();
void ISR_81();
void ISR_82();
void ISR_83();
void ISR_84();
void ISR_85();
void ISR_86();
void ISR_87();
void ISR_88();
void ISR_89();
void ISR_90();
void ISR_91();
void ISR_92();
void ISR_93();
void ISR_94();
void ISR_95();
void ISR_96();
void ISR_97();
void ISR_98();
void ISR_99();
void ISR_100();
void ISR_101();
void ISR_102();
void ISR_103();
void ISR_104();
void ISR_105();
void ISR_106();
void ISR_107();
void ISR_108();
void ISR_109();
void ISR_110();
void ISR_111();
void ISR_112();
void ISR_113();
void ISR_114();
void ISR_115();
void ISR_116();
void ISR_117();
void ISR_118();
void ISR_119();
void ISR_120();
void ISR_121();
void ISR_122();
void ISR_123();
void ISR_124();
void ISR_125();
void ISR_126();
void ISR_127();
void ISR_128();
void ISR_129();
void ISR_130();
void ISR_131();
void ISR_132();
void ISR_133();
void ISR_134();
void ISR_135();
void ISR_136();
void ISR_137();
void ISR_138();
void ISR_139();
void ISR_140();
void ISR_141();
void ISR_142();
void ISR_143();
void ISR_144();
void ISR_145();
void ISR_146();
void ISR_147();
void ISR_148();
void ISR_149();
void ISR_150();
void ISR_151();
void ISR_152();
void ISR_153();
void ISR_154();
void ISR_155();
void ISR_156();
void ISR_157();
void ISR_158();
void ISR_159();
void ISR_160();
void ISR_161();
void ISR_162();
void ISR_163();
void ISR_164();
void ISR_165();
void ISR_166();
void ISR_167();
void ISR_168();
void ISR_169();
void ISR_170();
void ISR_171();
void ISR_172();
void ISR_173();
void ISR_174();
void ISR_175();
void ISR_176();
void ISR_177();
void ISR_178();
void ISR_179();
void ISR_180();
void ISR_181();
void ISR_182();
void ISR_183();
void ISR_184();
void ISR_185();
void ISR_186();
void ISR_187();
void ISR_188();
void ISR_189();
void ISR_190();
void ISR_191();
void ISR_192();
void ISR_193();
void ISR_194();
void ISR_195();
void ISR_196();
void ISR_197();
void ISR_198();
void ISR_199();
void ISR_200();
void ISR_201();
void ISR_202();
void ISR_203();
void ISR_204();
void ISR_205();
void ISR_206();
void ISR_207();
void ISR_208();
void ISR_209();
void ISR_210();
void ISR_211();
void ISR_212();
void ISR_213();
void ISR_214();
void ISR_215();
void ISR_216();
void ISR_217();
void ISR_218();
void ISR_219();
void ISR_220();
void ISR_221();
void ISR_222();
void ISR_223();
void ISR_224();
void ISR_225();
void ISR_226();
void ISR_227();
void ISR_228();
void ISR_229();
void ISR_230();
void ISR_231();
void ISR_232();
void ISR_233();
void ISR_234();
void ISR_235();
void ISR_236();
void ISR_237();
void ISR_238();
void ISR_239();
void ISR_240();
void ISR_241();
void ISR_242();
void ISR_243();
void ISR_244();
void ISR_245();
void ISR_246();
void ISR_247();
void ISR_248();
void ISR_249();
void ISR_250();
void ISR_251();
void ISR_252();
void ISR_253();
void ISR_254();
void ISR_255();
#pragma endregion




/*
    ,--------------------,
    ; Exported functions ;
    '--------------------'
*/
#pragma region Exported functions

void set_ISR_Handler(int int_no, ISR_Handler handler) {
    if (int_no < 0 || int_no >= 256) {
        cli_error("Setting Invalid interrupt number: %d \n", int_no);
        return;
    }
    g_ISRHandlers[int_no] = handler;
}


void _sys_init_ISR() {
    int flag = IDT_FLAG_GATE_INTERRUPT | IDT_FLAG_RING_0 | IDT_FLAG_PRESENT;
    int syscall_flag = IDT_FLAG_GATE_INTERRUPT | IDT_FLAG_RING_3 | IDT_FLAG_PRESENT;
    uint16_t CODE_SEG = 0x08;
    _IDT_SetEntry(0, (uint32_t)ISR_0, CODE_SEG, flag);
    _IDT_SetEntry(1, (uint32_t)ISR_1, CODE_SEG, flag);
    _IDT_SetEntry(2, (uint32_t)ISR_2, CODE_SEG, flag);
    _IDT_SetEntry(3, (uint32_t)ISR_3, CODE_SEG, flag);
    _IDT_SetEntry(4, (uint32_t)ISR_4, CODE_SEG, flag);
    _IDT_SetEntry(5, (uint32_t)ISR_5, CODE_SEG, flag);
    _IDT_SetEntry(6, (uint32_t)ISR_6, CODE_SEG, flag);
    _IDT_SetEntry(7, (uint32_t)ISR_7, CODE_SEG, flag);
    _IDT_SetEntry(8, (uint32_t)ISR_8, CODE_SEG, flag);
    _IDT_SetEntry(9, (uint32_t)ISR_9, CODE_SEG, flag);
    _IDT_SetEntry(10, (uint32_t)ISR_10, CODE_SEG, flag);
    _IDT_SetEntry(11, (uint32_t)ISR_11, CODE_SEG, flag);
    _IDT_SetEntry(12, (uint32_t)ISR_12, CODE_SEG, flag);
    _IDT_SetEntry(13, (uint32_t)ISR_13, CODE_SEG, flag);
    _IDT_SetEntry(14, (uint32_t)ISR_14, CODE_SEG, flag);
    _IDT_SetEntry(15, (uint32_t)ISR_15, CODE_SEG, flag);
    _IDT_SetEntry(16, (uint32_t)ISR_16, CODE_SEG, flag);
    _IDT_SetEntry(17, (uint32_t)ISR_17, CODE_SEG, flag);
    _IDT_SetEntry(18, (uint32_t)ISR_18, CODE_SEG, flag);
    _IDT_SetEntry(19, (uint32_t)ISR_19, CODE_SEG, flag);
    _IDT_SetEntry(20, (uint32_t)ISR_20, CODE_SEG, flag);
    _IDT_SetEntry(21, (uint32_t)ISR_21, CODE_SEG, flag);
    _IDT_SetEntry(22, (uint32_t)ISR_22, CODE_SEG, flag);
    _IDT_SetEntry(23, (uint32_t)ISR_23, CODE_SEG, flag);
    _IDT_SetEntry(24, (uint32_t)ISR_24, CODE_SEG, flag);
    _IDT_SetEntry(25, (uint32_t)ISR_25, CODE_SEG, flag);
    _IDT_SetEntry(26, (uint32_t)ISR_26, CODE_SEG, flag);
    _IDT_SetEntry(27, (uint32_t)ISR_27, CODE_SEG, flag);
    _IDT_SetEntry(28, (uint32_t)ISR_28, CODE_SEG, flag);
    _IDT_SetEntry(29, (uint32_t)ISR_29, CODE_SEG, flag);
    _IDT_SetEntry(30, (uint32_t)ISR_30, CODE_SEG, flag);
    _IDT_SetEntry(31, (uint32_t)ISR_31, CODE_SEG, flag);
    _IDT_SetEntry(32, (uint32_t)ISR_32, CODE_SEG, flag);
    _IDT_SetEntry(33, (uint32_t)ISR_33, CODE_SEG, flag);
    _IDT_SetEntry(34, (uint32_t)ISR_34, CODE_SEG, flag);
    _IDT_SetEntry(35, (uint32_t)ISR_35, CODE_SEG, flag);
    _IDT_SetEntry(36, (uint32_t)ISR_36, CODE_SEG, flag);
    _IDT_SetEntry(37, (uint32_t)ISR_37, CODE_SEG, flag);
    _IDT_SetEntry(38, (uint32_t)ISR_38, CODE_SEG, flag);
    _IDT_SetEntry(39, (uint32_t)ISR_39, CODE_SEG, flag);
    _IDT_SetEntry(40, (uint32_t)ISR_40, CODE_SEG, flag);
    _IDT_SetEntry(41, (uint32_t)ISR_41, CODE_SEG, flag);
    _IDT_SetEntry(42, (uint32_t)ISR_42, CODE_SEG, flag);
    _IDT_SetEntry(43, (uint32_t)ISR_43, CODE_SEG, flag);
    _IDT_SetEntry(44, (uint32_t)ISR_44, CODE_SEG, flag);
    _IDT_SetEntry(45, (uint32_t)ISR_45, CODE_SEG, flag);
    _IDT_SetEntry(46, (uint32_t)ISR_46, CODE_SEG, flag);
    _IDT_SetEntry(47, (uint32_t)ISR_47, CODE_SEG, flag);
    _IDT_SetEntry(48, (uint32_t)ISR_48, CODE_SEG, flag);
    _IDT_SetEntry(49, (uint32_t)ISR_49, CODE_SEG, flag);
    _IDT_SetEntry(50, (uint32_t)ISR_50, CODE_SEG, flag);
    _IDT_SetEntry(51, (uint32_t)ISR_51, CODE_SEG, flag);
    _IDT_SetEntry(52, (uint32_t)ISR_52, CODE_SEG, flag);
    _IDT_SetEntry(53, (uint32_t)ISR_53, CODE_SEG, flag);
    _IDT_SetEntry(54, (uint32_t)ISR_54, CODE_SEG, flag);
    _IDT_SetEntry(55, (uint32_t)ISR_55, CODE_SEG, flag);
    _IDT_SetEntry(56, (uint32_t)ISR_56, CODE_SEG, flag);
    _IDT_SetEntry(57, (uint32_t)ISR_57, CODE_SEG, flag);
    _IDT_SetEntry(58, (uint32_t)ISR_58, CODE_SEG, flag);
    _IDT_SetEntry(59, (uint32_t)ISR_59, CODE_SEG, flag);
    _IDT_SetEntry(60, (uint32_t)ISR_60, CODE_SEG, flag);
    _IDT_SetEntry(61, (uint32_t)ISR_61, CODE_SEG, flag);
    _IDT_SetEntry(62, (uint32_t)ISR_62, CODE_SEG, flag);
    _IDT_SetEntry(63, (uint32_t)ISR_63, CODE_SEG, flag);
    _IDT_SetEntry(64, (uint32_t)ISR_64, CODE_SEG, flag);
    _IDT_SetEntry(65, (uint32_t)ISR_65, CODE_SEG, flag);
    _IDT_SetEntry(66, (uint32_t)ISR_66, CODE_SEG, flag);
    _IDT_SetEntry(67, (uint32_t)ISR_67, CODE_SEG, flag);
    _IDT_SetEntry(68, (uint32_t)ISR_68, CODE_SEG, flag);
    _IDT_SetEntry(69, (uint32_t)ISR_69, CODE_SEG, flag);
    _IDT_SetEntry(70, (uint32_t)ISR_70, CODE_SEG, flag);
    _IDT_SetEntry(71, (uint32_t)ISR_71, CODE_SEG, flag);
    _IDT_SetEntry(72, (uint32_t)ISR_72, CODE_SEG, flag);
    _IDT_SetEntry(73, (uint32_t)ISR_73, CODE_SEG, flag);
    _IDT_SetEntry(74, (uint32_t)ISR_74, CODE_SEG, flag);
    _IDT_SetEntry(75, (uint32_t)ISR_75, CODE_SEG, flag);
    _IDT_SetEntry(76, (uint32_t)ISR_76, CODE_SEG, flag);
    _IDT_SetEntry(77, (uint32_t)ISR_77, CODE_SEG, flag);
    _IDT_SetEntry(78, (uint32_t)ISR_78, CODE_SEG, flag);
    _IDT_SetEntry(79, (uint32_t)ISR_79, CODE_SEG, flag);
    _IDT_SetEntry(80, (uint32_t)ISR_80, CODE_SEG, flag);
    _IDT_SetEntry(81, (uint32_t)ISR_81, CODE_SEG, flag);
    _IDT_SetEntry(82, (uint32_t)ISR_82, CODE_SEG, flag);
    _IDT_SetEntry(83, (uint32_t)ISR_83, CODE_SEG, flag);
    _IDT_SetEntry(84, (uint32_t)ISR_84, CODE_SEG, flag);
    _IDT_SetEntry(85, (uint32_t)ISR_85, CODE_SEG, flag);
    _IDT_SetEntry(86, (uint32_t)ISR_86, CODE_SEG, flag);
    _IDT_SetEntry(87, (uint32_t)ISR_87, CODE_SEG, flag);
    _IDT_SetEntry(88, (uint32_t)ISR_88, CODE_SEG, flag);
    _IDT_SetEntry(89, (uint32_t)ISR_89, CODE_SEG, flag);
    _IDT_SetEntry(90, (uint32_t)ISR_90, CODE_SEG, flag);
    _IDT_SetEntry(91, (uint32_t)ISR_91, CODE_SEG, flag);
    _IDT_SetEntry(92, (uint32_t)ISR_92, CODE_SEG, flag);
    _IDT_SetEntry(93, (uint32_t)ISR_93, CODE_SEG, flag);
    _IDT_SetEntry(94, (uint32_t)ISR_94, CODE_SEG, flag);
    _IDT_SetEntry(95, (uint32_t)ISR_95, CODE_SEG, flag);
    _IDT_SetEntry(96, (uint32_t)ISR_96, CODE_SEG, flag);
    _IDT_SetEntry(97, (uint32_t)ISR_97, CODE_SEG, flag);
    _IDT_SetEntry(98, (uint32_t)ISR_98, CODE_SEG, flag);
    _IDT_SetEntry(99, (uint32_t)ISR_99, CODE_SEG, flag);
    _IDT_SetEntry(100, (uint32_t)ISR_100, CODE_SEG, flag);
    _IDT_SetEntry(101, (uint32_t)ISR_101, CODE_SEG, flag);
    _IDT_SetEntry(102, (uint32_t)ISR_102, CODE_SEG, flag);
    _IDT_SetEntry(103, (uint32_t)ISR_103, CODE_SEG, flag);
    _IDT_SetEntry(104, (uint32_t)ISR_104, CODE_SEG, flag);
    _IDT_SetEntry(105, (uint32_t)ISR_105, CODE_SEG, flag);
    _IDT_SetEntry(106, (uint32_t)ISR_106, CODE_SEG, flag);
    _IDT_SetEntry(107, (uint32_t)ISR_107, CODE_SEG, flag);
    _IDT_SetEntry(108, (uint32_t)ISR_108, CODE_SEG, flag);
    _IDT_SetEntry(109, (uint32_t)ISR_109, CODE_SEG, flag);
    _IDT_SetEntry(110, (uint32_t)ISR_110, CODE_SEG, flag);
    _IDT_SetEntry(111, (uint32_t)ISR_111, CODE_SEG, flag);
    _IDT_SetEntry(112, (uint32_t)ISR_112, CODE_SEG, flag);
    _IDT_SetEntry(113, (uint32_t)ISR_113, CODE_SEG, flag);
    _IDT_SetEntry(114, (uint32_t)ISR_114, CODE_SEG, flag);
    _IDT_SetEntry(115, (uint32_t)ISR_115, CODE_SEG, flag);
    _IDT_SetEntry(116, (uint32_t)ISR_116, CODE_SEG, flag);
    _IDT_SetEntry(117, (uint32_t)ISR_117, CODE_SEG, flag);
    _IDT_SetEntry(118, (uint32_t)ISR_118, CODE_SEG, flag);
    _IDT_SetEntry(119, (uint32_t)ISR_119, CODE_SEG, flag);
    _IDT_SetEntry(120, (uint32_t)ISR_120, CODE_SEG, flag);
    _IDT_SetEntry(121, (uint32_t)ISR_121, CODE_SEG, flag);
    _IDT_SetEntry(122, (uint32_t)ISR_122, CODE_SEG, flag);
    _IDT_SetEntry(123, (uint32_t)ISR_123, CODE_SEG, flag);
    _IDT_SetEntry(124, (uint32_t)ISR_124, CODE_SEG, flag);
    _IDT_SetEntry(125, (uint32_t)ISR_125, CODE_SEG, flag);
    _IDT_SetEntry(126, (uint32_t)ISR_126, CODE_SEG, flag);
    _IDT_SetEntry(127, (uint32_t)ISR_127, CODE_SEG, flag);
    _IDT_SetEntry(128, (uint32_t)ISR_128, CODE_SEG, syscall_flag);
    _IDT_SetEntry(129, (uint32_t)ISR_129, CODE_SEG, flag);
    _IDT_SetEntry(130, (uint32_t)ISR_130, CODE_SEG, flag);
    _IDT_SetEntry(131, (uint32_t)ISR_131, CODE_SEG, flag);
    _IDT_SetEntry(132, (uint32_t)ISR_132, CODE_SEG, flag);
    _IDT_SetEntry(133, (uint32_t)ISR_133, CODE_SEG, flag);
    _IDT_SetEntry(134, (uint32_t)ISR_134, CODE_SEG, flag);
    _IDT_SetEntry(135, (uint32_t)ISR_135, CODE_SEG, flag);
    _IDT_SetEntry(136, (uint32_t)ISR_136, CODE_SEG, flag);
    _IDT_SetEntry(137, (uint32_t)ISR_137, CODE_SEG, flag);
    _IDT_SetEntry(138, (uint32_t)ISR_138, CODE_SEG, flag);
    _IDT_SetEntry(139, (uint32_t)ISR_139, CODE_SEG, flag);
    _IDT_SetEntry(140, (uint32_t)ISR_140, CODE_SEG, flag);
    _IDT_SetEntry(141, (uint32_t)ISR_141, CODE_SEG, flag);
    _IDT_SetEntry(142, (uint32_t)ISR_142, CODE_SEG, flag);
    _IDT_SetEntry(143, (uint32_t)ISR_143, CODE_SEG, flag);
    _IDT_SetEntry(144, (uint32_t)ISR_144, CODE_SEG, flag);
    _IDT_SetEntry(145, (uint32_t)ISR_145, CODE_SEG, flag);
    _IDT_SetEntry(146, (uint32_t)ISR_146, CODE_SEG, flag);
    _IDT_SetEntry(147, (uint32_t)ISR_147, CODE_SEG, flag);
    _IDT_SetEntry(148, (uint32_t)ISR_148, CODE_SEG, flag);
    _IDT_SetEntry(149, (uint32_t)ISR_149, CODE_SEG, flag);
    _IDT_SetEntry(150, (uint32_t)ISR_150, CODE_SEG, flag);
    _IDT_SetEntry(151, (uint32_t)ISR_151, CODE_SEG, flag);
    _IDT_SetEntry(152, (uint32_t)ISR_152, CODE_SEG, flag);
    _IDT_SetEntry(153, (uint32_t)ISR_153, CODE_SEG, flag);
    _IDT_SetEntry(154, (uint32_t)ISR_154, CODE_SEG, flag);
    _IDT_SetEntry(155, (uint32_t)ISR_155, CODE_SEG, flag);
    _IDT_SetEntry(156, (uint32_t)ISR_156, CODE_SEG, flag);
    _IDT_SetEntry(157, (uint32_t)ISR_157, CODE_SEG, flag);
    _IDT_SetEntry(158, (uint32_t)ISR_158, CODE_SEG, flag);
    _IDT_SetEntry(159, (uint32_t)ISR_159, CODE_SEG, flag);
    _IDT_SetEntry(160, (uint32_t)ISR_160, CODE_SEG, flag);
    _IDT_SetEntry(161, (uint32_t)ISR_161, CODE_SEG, flag);
    _IDT_SetEntry(162, (uint32_t)ISR_162, CODE_SEG, flag);
    _IDT_SetEntry(163, (uint32_t)ISR_163, CODE_SEG, flag);
    _IDT_SetEntry(164, (uint32_t)ISR_164, CODE_SEG, flag);
    _IDT_SetEntry(165, (uint32_t)ISR_165, CODE_SEG, flag);
    _IDT_SetEntry(166, (uint32_t)ISR_166, CODE_SEG, flag);
    _IDT_SetEntry(167, (uint32_t)ISR_167, CODE_SEG, flag);
    _IDT_SetEntry(168, (uint32_t)ISR_168, CODE_SEG, flag);
    _IDT_SetEntry(169, (uint32_t)ISR_169, CODE_SEG, flag);
    _IDT_SetEntry(170, (uint32_t)ISR_170, CODE_SEG, flag);
    _IDT_SetEntry(171, (uint32_t)ISR_171, CODE_SEG, flag);
    _IDT_SetEntry(172, (uint32_t)ISR_172, CODE_SEG, flag);
    _IDT_SetEntry(173, (uint32_t)ISR_173, CODE_SEG, flag);
    _IDT_SetEntry(174, (uint32_t)ISR_174, CODE_SEG, flag);
    _IDT_SetEntry(175, (uint32_t)ISR_175, CODE_SEG, flag);
    _IDT_SetEntry(176, (uint32_t)ISR_176, CODE_SEG, flag);
    _IDT_SetEntry(177, (uint32_t)ISR_177, CODE_SEG, flag);
    _IDT_SetEntry(178, (uint32_t)ISR_178, CODE_SEG, flag);
    _IDT_SetEntry(179, (uint32_t)ISR_179, CODE_SEG, flag);
    _IDT_SetEntry(180, (uint32_t)ISR_180, CODE_SEG, flag);
    _IDT_SetEntry(181, (uint32_t)ISR_181, CODE_SEG, flag);
    _IDT_SetEntry(182, (uint32_t)ISR_182, CODE_SEG, flag);
    _IDT_SetEntry(183, (uint32_t)ISR_183, CODE_SEG, flag);
    _IDT_SetEntry(184, (uint32_t)ISR_184, CODE_SEG, flag);
    _IDT_SetEntry(185, (uint32_t)ISR_185, CODE_SEG, flag);
    _IDT_SetEntry(186, (uint32_t)ISR_186, CODE_SEG, flag);
    _IDT_SetEntry(187, (uint32_t)ISR_187, CODE_SEG, flag);
    _IDT_SetEntry(188, (uint32_t)ISR_188, CODE_SEG, flag);
    _IDT_SetEntry(189, (uint32_t)ISR_189, CODE_SEG, flag);
    _IDT_SetEntry(190, (uint32_t)ISR_190, CODE_SEG, flag);
    _IDT_SetEntry(191, (uint32_t)ISR_191, CODE_SEG, flag);
    _IDT_SetEntry(192, (uint32_t)ISR_192, CODE_SEG, flag);
    _IDT_SetEntry(193, (uint32_t)ISR_193, CODE_SEG, flag);
    _IDT_SetEntry(194, (uint32_t)ISR_194, CODE_SEG, flag);
    _IDT_SetEntry(195, (uint32_t)ISR_195, CODE_SEG, flag);
    _IDT_SetEntry(196, (uint32_t)ISR_196, CODE_SEG, flag);
    _IDT_SetEntry(197, (uint32_t)ISR_197, CODE_SEG, flag);
    _IDT_SetEntry(198, (uint32_t)ISR_198, CODE_SEG, flag);
    _IDT_SetEntry(199, (uint32_t)ISR_199, CODE_SEG, flag);
    _IDT_SetEntry(200, (uint32_t)ISR_200, CODE_SEG, flag);
    _IDT_SetEntry(201, (uint32_t)ISR_201, CODE_SEG, flag);
    _IDT_SetEntry(202, (uint32_t)ISR_202, CODE_SEG, flag);
    _IDT_SetEntry(203, (uint32_t)ISR_203, CODE_SEG, flag);
    _IDT_SetEntry(204, (uint32_t)ISR_204, CODE_SEG, flag);
    _IDT_SetEntry(205, (uint32_t)ISR_205, CODE_SEG, flag);
    _IDT_SetEntry(206, (uint32_t)ISR_206, CODE_SEG, flag);
    _IDT_SetEntry(207, (uint32_t)ISR_207, CODE_SEG, flag);
    _IDT_SetEntry(208, (uint32_t)ISR_208, CODE_SEG, flag);
    _IDT_SetEntry(209, (uint32_t)ISR_209, CODE_SEG, flag);
    _IDT_SetEntry(210, (uint32_t)ISR_210, CODE_SEG, flag);
    _IDT_SetEntry(211, (uint32_t)ISR_211, CODE_SEG, flag);
    _IDT_SetEntry(212, (uint32_t)ISR_212, CODE_SEG, flag);
    _IDT_SetEntry(213, (uint32_t)ISR_213, CODE_SEG, flag);
    _IDT_SetEntry(214, (uint32_t)ISR_214, CODE_SEG, flag);
    _IDT_SetEntry(215, (uint32_t)ISR_215, CODE_SEG, flag);
    _IDT_SetEntry(216, (uint32_t)ISR_216, CODE_SEG, flag);
    _IDT_SetEntry(217, (uint32_t)ISR_217, CODE_SEG, flag);
    _IDT_SetEntry(218, (uint32_t)ISR_218, CODE_SEG, flag);
    _IDT_SetEntry(219, (uint32_t)ISR_219, CODE_SEG, flag);
    _IDT_SetEntry(220, (uint32_t)ISR_220, CODE_SEG, flag);
    _IDT_SetEntry(221, (uint32_t)ISR_221, CODE_SEG, flag);
    _IDT_SetEntry(222, (uint32_t)ISR_222, CODE_SEG, flag);
    _IDT_SetEntry(223, (uint32_t)ISR_223, CODE_SEG, flag);
    _IDT_SetEntry(224, (uint32_t)ISR_224, CODE_SEG, flag);
    _IDT_SetEntry(225, (uint32_t)ISR_225, CODE_SEG, flag);
    _IDT_SetEntry(226, (uint32_t)ISR_226, CODE_SEG, flag);
    _IDT_SetEntry(227, (uint32_t)ISR_227, CODE_SEG, flag);
    _IDT_SetEntry(228, (uint32_t)ISR_228, CODE_SEG, flag);
    _IDT_SetEntry(229, (uint32_t)ISR_229, CODE_SEG, flag);
    _IDT_SetEntry(230, (uint32_t)ISR_230, CODE_SEG, flag);
    _IDT_SetEntry(231, (uint32_t)ISR_231, CODE_SEG, flag);
    _IDT_SetEntry(232, (uint32_t)ISR_232, CODE_SEG, flag);
    _IDT_SetEntry(233, (uint32_t)ISR_233, CODE_SEG, flag);
    _IDT_SetEntry(234, (uint32_t)ISR_234, CODE_SEG, flag);
    _IDT_SetEntry(235, (uint32_t)ISR_235, CODE_SEG, flag);
    _IDT_SetEntry(236, (uint32_t)ISR_236, CODE_SEG, flag);
    _IDT_SetEntry(237, (uint32_t)ISR_237, CODE_SEG, flag);
    _IDT_SetEntry(238, (uint32_t)ISR_238, CODE_SEG, flag);
    _IDT_SetEntry(239, (uint32_t)ISR_239, CODE_SEG, flag);
    _IDT_SetEntry(240, (uint32_t)ISR_240, CODE_SEG, flag);
    _IDT_SetEntry(241, (uint32_t)ISR_241, CODE_SEG, flag);
    _IDT_SetEntry(242, (uint32_t)ISR_242, CODE_SEG, flag);
    _IDT_SetEntry(243, (uint32_t)ISR_243, CODE_SEG, flag);
    _IDT_SetEntry(244, (uint32_t)ISR_244, CODE_SEG, flag);
    _IDT_SetEntry(245, (uint32_t)ISR_245, CODE_SEG, flag);
    _IDT_SetEntry(246, (uint32_t)ISR_246, CODE_SEG, flag);
    _IDT_SetEntry(247, (uint32_t)ISR_247, CODE_SEG, flag);
    _IDT_SetEntry(248, (uint32_t)ISR_248, CODE_SEG, flag);
    _IDT_SetEntry(249, (uint32_t)ISR_249, CODE_SEG, flag);
    _IDT_SetEntry(250, (uint32_t)ISR_250, CODE_SEG, flag);
    _IDT_SetEntry(251, (uint32_t)ISR_251, CODE_SEG, flag);
    _IDT_SetEntry(252, (uint32_t)ISR_252, CODE_SEG, flag);
    _IDT_SetEntry(253, (uint32_t)ISR_253, CODE_SEG, flag);
    _IDT_SetEntry(254, (uint32_t)ISR_254, CODE_SEG, flag);
    _IDT_SetEntry(255, (uint32_t)ISR_255, CODE_SEG, flag);


    default_ISR_handlers();
}

#pragma endregion