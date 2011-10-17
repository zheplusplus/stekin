#ifndef __STEKIN_OUTPUT_STATEMENT_WRITER_H__
#define __STEKIN_OUTPUT_STATEMENT_WRITER_H__

#include <string>


namespace output {

    void kwReturn();
    void returnNothing();
    void initThisLevel(int offset, std::string const& type_exported_name);
    void addResEntry(int entry_offset);

    void branchIf();
    void branchElse();

    void blockBegin();
    void blockEnd();
    void endOfStatement();

}

#endif /* __STEKIN_OUTPUT_STATEMENT_WRITER_H__ */
