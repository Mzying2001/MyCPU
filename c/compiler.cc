/**
 * 汇编器
 */

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include "asm.h"
#include "pin.h"

// 二地址指令
std::map<std::string, uint8_t> op2 = {
    {"MOV", MOV},
    {"ADD", ADD},
    {"SUB", SUB},
    {"AND", AND},
    {"OR", OR},
    {"XOR", XOR},
    {"CMP", CMP},
};

// 一地址指令
std::map<std::string, uint8_t> op1 = {
    {"INC", INC},
    {"DEC", DEC},
    {"NOT", NOT},
    {"JMP", JMP},
    {"JO", JO},
    {"JZ", JZ},
    {"JP", JP},
    {"JNO", JNO},
    {"JNZ", JNZ},
    {"JNP", JNP},
    {"PUSH", PUSH},
    {"POP", POP},
    {"CALL", CALL},
    {"INT", INT},
};

// 零地址指令
std::map<std::string, uint8_t> op0 = {
    {"NOP", NOP},
    {"RET", RET},
    {"IRET", IRET},
    {"STI", STI},
    {"CLI", CLI},
    {"HLT", HLT},
};

// 寄存器
std::map<std::string, uint8_t> regs = {
    {"MSR", MSR},
    {"MAR", MAR},
    {"MDR", MDR},
    {"RAM", RAM},
    {"IR", IR},
    {"DST", DST},
    {"SRC", SRC},
    {"A", A},
    {"B", B},
    {"C", C},
    {"D", D},
    {"DI", DI},
    {"SI", SI},
    {"SP", SP},
    {"BP", BP},
    {"CS", CS},
    {"DS", DS},
    {"SS", SS},
    {"ES", ES},
    {"VEC", VEC},
    {"T1", T1},
    {"T2", T2},
};

/*============================================================*/

/// @brief 删除字符串首位空白字符
/// @param str
/// @return
std::string Trim(const std::string &str)
{
    if (str.empty())
        return str;
    size_t beg = 0;
    size_t end = str.length() - 1;
    while (beg <= end && std::isspace(str[beg]))
        ++beg;
    while (end >= beg && std::isspace(str[end]))
        --end;
    return end >= beg ? str.substr(beg, end - beg + 1) : "";
}

/// @brief 将字符串中的字符转换为大写
/// @param str
void ToUpper(std::string &str)
{
    size_t i = str.size();
    while (i--)
        str[i] = std::toupper(str[i]);
}

/// @brief 经预处理后的代码行
struct CodeLine
{
    size_t lineno;
    std::string line;
    CodeLine(size_t lineno, const std::string &line) : lineno(lineno)
    {
        size_t i = line.find(';');
        this->line = Trim((i == std::string::npos) ? line : line.substr(0, i));
        ToUpper(this->line);
    }

    /// @brief 打印当前行出现的错误并显示行数和本行内容
    /// @param msg 错误信息
    void PrintSyntaxError(const std::string &msg) const
    {
        std::cout << "syntax error: " << msg << std::endl
                  << "    " << lineno << " | " << line << std::endl;
    }

    /// @brief 判断是否为空行，只含注释也视为空行
    /// @param line
    /// @return
    static bool IsEmpityLine(const std::string &line)
    {
        for (char c : line)
            if (!std::isspace(c))
                return c == ';';
        return true;
    }

    /// @brief 逐行读取文件，并删除空白行、前后空格和注释
    /// @param src
    /// @return
    static std::vector<CodeLine> ReadFile(std::ifstream &src)
    {
        std::vector<CodeLine> file;
        size_t lineno = 0;
        std::string line;
        while (std::getline(src, line))
        {
            ++lineno;
            if (!CodeLine::IsEmpityLine(line))
                file.emplace_back(lineno, line);
        }
        return file;
    }
};

/// @brief 表示一个指令或者标签
struct AsmInstruction
{
    enum Type
    {
        LABEL,      // 标签
        TWOADDR,    // 二地址指令
        ONEADDR,    // 一地址指令
        ZEROADDR,   // 零地址指令
        SYNTAXERROR // 语法错误
    } type;
    std::string name; // op or label
    std::string dst;
    std::string src;

    /// @brief 解析指令
    /// @param codeline
    /// @return
    static AsmInstruction Parse(const CodeLine &codeline)
    {
        AsmInstruction ret;

        static const int STATE_START = 0;          // 开始状态
        static const int STATE_ERROR = 1;          // 发生错误
        static const int STATE_SUCCESS = 2;        // 解析成功
        static const int STATE_ON_FIRST_WORD = 3;  // 正在读第一个词
        static const int STATE_END_FIRST_WORD = 4; // 读完第一个词
        static const int STATE_END_LABEL = 5;      // 读完标签
        static const int STATE_ON_DST = 6;         // 正在读dst
        static const int STATE_END_DST = 7;        // 读dst结束
        static const int STATE_START_SRC = 8;      // 开始读src
        static const int STATE_ON_SRC = 9;         // 正在读src

        std::string buf;
        int state = STATE_START;
        const char *p = codeline.line.c_str();
        while (1)
        {
            char c = *p;
            switch (state)
            {
            case STATE_START:
                state = STATE_ON_FIRST_WORD; // 开始读第一个单词，可能是标签或者指令
                break;

            case STATE_ON_FIRST_WORD:
                if (std::isalpha(c))
                    ++p, buf += c; // 继续读第一个单词
                else
                    state = STATE_END_FIRST_WORD; // 读完第一个单词
                break;

            case STATE_END_FIRST_WORD:
                if (buf.size() == 0)
                {
                    // 无效op或label
                    ret.type == SYNTAXERROR;
                    state = STATE_ERROR;
                    codeline.PrintSyntaxError("invalid op or label name");
                }
                else if (std::isblank(c))
                    ++p; // 忽略空白字符
                else if (c == 0)
                {
                    // 读取到零地址指令
                    ret.type = ZEROADDR;
                    ret.name = buf;
                    state = STATE_SUCCESS;
                }
                else if (c == ':')
                    ++p, state = STATE_END_LABEL; // 读取到标签
                else
                {
                    // 当前读到的是指令，开始读dst
                    ret.name = buf;
                    buf.clear();
                    state = STATE_ON_DST;
                }
                break;

            case STATE_END_LABEL:
                if (std::isblank(c))
                    ++p; // 忽略空格
                else if (c == 0)
                {
                    // 读取到标签
                    ret.type = LABEL;
                    ret.name = buf;
                    state = STATE_SUCCESS;
                    break;
                }
                else
                {
                    // 语法错误：标签后面还有东西
                    ret.type = SYNTAXERROR;
                    state = STATE_ERROR;
                    codeline.PrintSyntaxError(std::string({'\'', c, 0}) + "\' unexcepted");
                }
                break;

            case STATE_ON_DST:
                if (std::isblank(c) || c == 0 || c == ',')
                    state = STATE_END_DST; // 读取dst结束
                else
                    ++p, buf += c; // 继续读取dst
                break;

            case STATE_END_DST:
                if (std::isblank(c))
                    ++p; // 忽略空格
                else if (c == 0)
                {
                    // 读取到一地址指令
                    ret.type = ONEADDR;
                    ret.dst = buf;
                    state = STATE_SUCCESS;
                }
                else if (c == ',')
                {
                    // 开始读src
                    ++p;
                    ret.dst = buf;
                    buf.clear();
                    state = STATE_START_SRC;
                }
                else
                {
                    // 语法错误：dst和src没有用逗号隔开
                    ret.type = SYNTAXERROR;
                    state = STATE_ERROR;
                    codeline.PrintSyntaxError("dst and src should be separated by \',\'");
                }
                break;

            case STATE_START_SRC:
                if (std::isblank(c))
                    ++p; // 忽略空格
                else if (c == 0)
                {
                    // 语法错误：src为空
                    ret.type = SYNTAXERROR;
                    state = STATE_ERROR;
                    codeline.PrintSyntaxError("src is empty");
                }
                else
                    state = STATE_ON_SRC; // 开始读src
                break;

            case STATE_ON_SRC:
                if (c == 0)
                {
                    // 读完src
                    ret.type = TWOADDR;
                    ret.src = buf;
                    state = STATE_SUCCESS;
                }
                else if (std::isblank(c) || c == ',')
                {
                    // 语法错误：多于两个地址
                    ret.type = SYNTAXERROR;
                    state = STATE_ERROR;
                    codeline.PrintSyntaxError("supports up to two addresses");
                }
                else
                    ++p, buf += c;
                break;
            }
            if (state == STATE_SUCCESS || state == STATE_ERROR)
                break; // 状态机结束
        }

        return ret;
    }
};

/*============================================================*/

/// @brief 判断字符串内容是否为数字
/// @param str
/// @return
bool IsInt(const std::string &str)
{
    for (char c : str)
        if (!std::isdigit(c))
            return false;
    return true;
}

/// @brief 判断字符串内容是否为十六位数字
/// @param str
/// @return
bool IsHexInt(const std::string &str)
{
    if (str.size() < 3 || str[0] != '0' || std::tolower(str[1]) != 'x')
        return false;
    for (size_t i = 2; i < str.size(); ++i)
        if (!std::isxdigit(str[i]))
            return false;
    return true;
}

/// @brief 获取寻址方式
/// @param addr 要获取寻址方式的地址
/// @param refam 地址的寻址方式
/// @param refval 地址的内容
/// @return
bool GetAM(const std::string &addr, uint8_t &refam, uint8_t &refval)
{
    if (addr.size() > 2 && *addr.begin() == '[' && *addr.rbegin() == ']')
    {
        std::string paddr = addr.substr(1, addr.size() - 2);
        if (regs.count(paddr))
        {
            refam = AM_RAM;
            refval = regs[paddr];
            return true;
        }
        if (IsInt(paddr))
        {
            refam = AM_DIR;
            refval = std::atoi(paddr.c_str());
            return true;
        }
        if (IsHexInt(paddr))
        {
            int val;
            sscanf(paddr.c_str(), "%x", &val);
            refam = AM_DIR;
            refval = val;
            return true;
        }
    }
    else
    {
        if (regs.count(addr))
        {
            refam = AM_REG;
            refval = regs[addr];
            return true;
        }
        if (IsInt(addr))
        {
            refam = AM_INS;
            refval = std::atoi(addr.c_str());
            return true;
        }
        if (IsHexInt(addr))
        {
            int val;
            sscanf(addr.c_str(), "%x", &val);
            refam = AM_INS;
            refval = val;
            return true;
        }
    }
    return false;
}

/// @brief 检测二地址指令的寻址方式是否符合要求
/// @param amd
/// @param ams
/// @return
bool CheckAM(uint8_t amd, uint8_t ams)
{
    return amd == AM_REG                      // e.g MOV A, ...
           || amd == AM_DIR && ams == AM_REG  // e.g MOV [5], A
           || amd == AM_DIR && ams == AM_INS  // e.g MOV [5], 10
           || amd == AM_RAM && ams == AM_REG  // e.g MOV [A], B
           || amd == AM_RAM && ams == AM_INS; // e.g MOV [A], 10
}

/// @brief 编译程序
/// @param file
/// @param out
/// @return
bool Compile(const std::vector<CodeLine> &file, std::ofstream &out)
{
    std::vector<AsmInstruction> instructions;
    for (const CodeLine &codeline : file)
        instructions.emplace_back(AsmInstruction::Parse(codeline)); // 逐行解析

    // 统计语法错误，存在错误则编译失败
    int cnterr = 0;
    for (const AsmInstruction &item : instructions)
        cnterr += item.type == AsmInstruction::SYNTAXERROR;
    if (cnterr)
    {
        std::cout << "compile error: " << cnterr << " syntax error(s) found" << std::endl;
        return false;
    }

    size_t programsize = 0;                 // 程序大小
    std::map<std::string, size_t> labelpos; // 标签所在位置
    for (size_t i = 0; i < instructions.size(); ++i)
    {
        const AsmInstruction &item = instructions[i];
        if (item.type == AsmInstruction::LABEL)
        {
            if (!labelpos.count(item.name))
            {
                labelpos[item.name] = programsize;
            }
            else
            {
                file[i].PrintSyntaxError("label \"" + item.name + "\" defined repeatedly");
                return false;
            }
        }
        else
        {
            programsize += 3; // 指令长度固定为3
        }
    }

    for (size_t i = 0; i < instructions.size(); ++i)
    {
        uint8_t op, dst, src, amd, ams, ir;
        AsmInstruction &item = instructions[i];
        if (item.type == AsmInstruction::LABEL)
        {
            continue; // 忽略标签
        }
        else if (item.type == AsmInstruction::TWOADDR)
        {
            // 二地址指令 1xxxaabb
            if (!op2.count(item.name))
            {
                file[i].PrintSyntaxError("unsupported op \"" + item.name + "\"");
                return false;
            }
            op = op2[item.name];
            if (!GetAM(item.dst, amd, dst))
            {
                file[i].PrintSyntaxError("invalid dst");
                return false;
            }
            if (!GetAM(item.src, ams, src))
            {
                file[i].PrintSyntaxError("invalid src");
                return false;
            }
            if (!CheckAM(amd, ams))
            {
                file[i].PrintSyntaxError("invalid addressing mode");
                return false;
            }
            ir = op | (amd << 2) | ams;
            out.write((char *)&ir, 1);
            out.write((char *)&dst, 1);
            out.write((char *)&src, 1);
        }
        else if (item.type == AsmInstruction::ONEADDR)
        {
            // 一地址指令 01xxxxaa
            if (!op1.count(item.name))
            {
                file[i].PrintSyntaxError("unsupported op \"" + item.name + "\"");
                return false;
            }
            op = op1[item.name];
            if ((op == JMP                                                                  //
                 || op == JO || op == JZ || op == JP || op == JNO || op == JNZ || op == JNP //
                 || op == CALL || op == INT)                                                //
                && labelpos.count(item.dst))
            {
                item.dst = std::to_string(labelpos[item.dst]); // 跳转指令替换标签为立即数
            }
            if (!GetAM(item.dst, amd, dst))
            {
                file[i].PrintSyntaxError("invalid dst");
                return false;
            }
            src = 0;
            ir = op | amd;
            out.write((char *)&ir, 1);
            out.write((char *)&dst, 1);
            out.write((char *)&src, 1);
        }
        else if (item.type == AsmInstruction::ZEROADDR)
        {
            // 零地址指令
            if (!op0.count(item.name))
            {
                file[i].PrintSyntaxError("unsupported instruction \"" + item.name + "\"");
                return false;
            }
            op = op0[item.name];
            dst = src = 0;
            ir = op;
            out.write((char *)&ir, 1);
            out.write((char *)&dst, 1);
            out.write((char *)&src, 1);
        }
    }

    return true;
}

/*============================================================*/

int main(int argc, char *argv[])
{
    std::string srcfile;
    std::string outfile;

    if (argc == 3)
    {
        srcfile = argv[1];
        outfile = argv[2];
    }
    else if (argc == 2)
    {
        srcfile = argv[1];
        outfile = srcfile + ".bin";
    }
    else
    {
        std::cout << "compiler [srcfile] [outfile]" << std::endl
                  << std::endl
                  << "  srcfile: source file path" << std::endl
                  << "  outfile: output file path" << std::endl
                  << std::endl;
        return 0;
    }

    std::ifstream src;
    std::ofstream out;

    src.open(srcfile, std::ios::in);
    if (!src.is_open())
    {
        std::cout << "error: unable to open source file" << std::endl;
        return 0;
    }

    out.open(outfile, std::ios::out | std::ios::binary);
    if (!out.is_open())
    {
        std::cout << "error: unable to open output file" << std::endl;
        src.close();
        return 0;
    }

    if (Compile(CodeLine::ReadFile(src), out))
        std::cout << "done" << std::endl;

    src.close();
    out.close();

    return 0;
}
