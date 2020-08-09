#ifndef SERVER_STORAGE_EXCEPTIONS_H
#define SERVER_STORAGE_EXCEPTIONS_H

#include <exception>
#include <string>

// Base class for all exceptions thrown by all storage methods.
class StorageException : public std::exception {
 public:
   StorageException(const std::string& msg) : msg_(msg) {}
   const char * what () const throw ()
   {
     return msg_.c_str();
   }
 private:
   std::string msg_;
};

// Exception class wrapping Sqlite errors.
class SqliteException : public StorageException {
public:
  SqliteException(const std::string& msg) : StorageException(msg) {}
};

// Exception class indicating that the data requested was not found.
class NotFoundException : public StorageException {
public:
  NotFoundException(const std::string& msg) : StorageException(msg) {}
};

#endif
