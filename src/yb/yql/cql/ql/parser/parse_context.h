//--------------------------------------------------------------------------------------------------
// Copyright (c) YugaByte, Inc.
//
// Licensed under the Apache License, Version 2.0 (the "License"); you may not use this file except
// in compliance with the License.  You may obtain a copy of the License at
//
// http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software distributed under the License
// is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express
// or implied.  See the License for the specific language governing permissions and limitations
// under the License.
//
//
// Entry point for the parsing process.
//--------------------------------------------------------------------------------------------------

#ifndef YB_YQL_CQL_QL_PARSER_PARSE_CONTEXT_H_
#define YB_YQL_CQL_QL_PARSER_PARSE_CONTEXT_H_

#include "yb/yql/cql/ql/parser/location.h"
#include "yb/yql/cql/ql/ptree/process_context.h"
#include "yb/yql/cql/ql/ptree/pt_expr.h"

namespace yb {
namespace ql {

// Parsing context.
class ParseContext : public ProcessContext {
 public:
  //------------------------------------------------------------------------------------------------
  // Public types.
  typedef std::unique_ptr<ParseContext> UniPtr;
  typedef std::unique_ptr<const ParseContext> UniPtrConst;

  //------------------------------------------------------------------------------------------------
  // Constructor & destructor.
  ParseContext(const char *stmt = "",
               size_t stmt_len = 0,
               bool reparsed = false,
               std::shared_ptr<MemTracker> mem_tracker = nullptr);
  virtual ~ParseContext();

  // Read a maximum of 'max_size' bytes from SQL statement of this parsing context into the
  // provided buffer 'buf'. Scanner will call this function when looking for next token.
  size_t Read(char* buf, size_t max_size);

  // Add a bind variable.
  void AddBindVariable(PTBindVar *var) {
    bind_variables_.insert(var);
  }

  // Return the list of bind variables found during parsing.
  void GetBindVariables(MCVector<PTBindVar*> *vars);

  // Handling parsing warning.
  void Warn(const location& l, const char *m, ErrorCode error_code) {
    ProcessContext::Warn(Location(l), m, error_code);
  }

  // Handling parsing error.
  CHECKED_STATUS Error(const location& l,
                       const char *m,
                       ErrorCode error_code,
                       const char* token = nullptr) {
    return ProcessContext::Error(Location(l), m, error_code, token);
  }
  CHECKED_STATUS Error(const location& l, const char *m, const char* token = nullptr) {
    return ProcessContext::Error(Location(l), m, token);
  }
  CHECKED_STATUS Error(const location& l, ErrorCode error_code, const char* token = nullptr) {
    return ProcessContext::Error(Location(l), error_code, token);
  }

  // Access function for ql_file_.
  std::istream *ql_file() {
    return ql_file_ == nullptr ? nullptr : ql_file_.get();
  }

  // Access function for trace_scanning_.
  bool trace_scanning() const {
    return trace_scanning_;
  }

  // Access function for trace_parsing_.
  bool trace_parsing() const {
    return trace_parsing_;
  }

 private:
  // List of bind variables in the statement being parsed ordered by the ordinal position in the
  // statement.
  MCSet<PTBindVar*, PTBindVar::SetCmp> bind_variables_;

  //------------------------------------------------------------------------------------------------
  // We don't use istream (i.e. file) as input when parsing. In the future, if we also support file
  // as an SQL input, we need to define a constructor that takes a file as input and initializes
  // "ql_file_" accordingly.
  std::unique_ptr<std::istream> ql_file_;

  //------------------------------------------------------------------------------------------------
  // NOTE: All entities below this line in this modules are copies of PostgreQL's code. We made
  // some minor changes to avoid lint errors such as using '{' for if blocks, change the comment
  // style from '/**/' to '//', and post-fix data members with "_".
  //------------------------------------------------------------------------------------------------
  size_t stmt_offset_;         // SQL statement to be scanned.
  bool trace_scanning_;        // Scanner trace flag.
  bool trace_parsing_;         // Parser trace flag.
};

}  // namespace ql
}  // namespace yb

#endif  // YB_YQL_CQL_QL_PARSER_PARSE_CONTEXT_H_