/* Copyright (C) 2016-2018 Alibaba Group Holding Limited

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

#ifndef XDL_CORE_IO_PARSER_H_
#define XDL_CORE_IO_PARSER_H_

#include "xdl/data_io/sgroup.h"
#include "xdl/data_io/schema.h"
#include "xdl/data_io/fs/file_system.h"
#include "xdl/core/utils/logging.h"

#include <sstream>

namespace xdl {
namespace io {

/*!\brief the read args */
struct ReadParam {
  ~ReadParam() {
    delete path_;
    delete ant_;
    ant_ = nullptr;
  }
  size_t begin_ = 0;
  size_t end_ = 0;
  size_t epoch_ = 0;
  const char *path_;
  IOAnt *ant_ = nullptr;

  std::string DebugString() const {
    std::stringstream ss;
    ss << " begin=" << begin_
       << " end=" << end_
       << " epoch=" << epoch_
       << " path=" << path_ 
       << " io_ant=" << (void*)ant_;
    return ss.str();
  }
};

class Parse {
 public:
  Parse(const Schema *schema) : schema_(schema) {}
  virtual ~Parse(){}

  virtual bool InitMeta(const std::string &contents) { return true;};
  virtual SGroup *Run(const char *str, size_t len) = 0;
  virtual ssize_t GetSize(const char *str, size_t len) = 0;

 protected:
  const Schema *schema_ = nullptr;
};

class Parser {
 public:
  static const size_t kReadSize = 256 * 1024;
  static const size_t kBufSize = 128 * 1024 * 1024;
  Parser(ParserType type, const Schema *schema);
  virtual ~Parser() {}

  bool InitMeta(const std::string &meta);
  bool Init(ReadParam *rparam);
  SGroup *Run();
  SGroup *Run1();
  bool Shutdown();

 protected:
  bool running_ = true;
  const Schema *schema_ = nullptr;
  SGroup *Read2Parse();

  Parse *parse_ = nullptr;
  ReadParam *rparam_ = nullptr;
  char buf_[kBufSize] = {0};
  off_t begin_ = 0;
  off_t end_ = 0;
};

}  // namespace io
}  // namespace xdl

#endif  // XDL_CORE_IO_OP_H_
