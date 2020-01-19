// Generated by the protocol buffer compiler.  DO NOT EDIT!
// source: GameLogo.proto

#ifndef PROTOBUF_GameLogo_2eproto__INCLUDED
#define PROTOBUF_GameLogo_2eproto__INCLUDED

#include <string>

#include <google/protobuf/stubs/common.h>

#if GOOGLE_PROTOBUF_VERSION < 2006000
#error This file was generated by a newer version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please update
#error your headers.
#endif
#if 2006001 < GOOGLE_PROTOBUF_MIN_PROTOC_VERSION
#error This file was generated by an older version of protoc which is
#error incompatible with your Protocol Buffer headers.  Please
#error regenerate this file with a newer version of protoc.
#endif

#include <google/protobuf/generated_message_util.h>
#include <google/protobuf/message_lite.h>
#include <google/protobuf/repeated_field.h>
#include <google/protobuf/extension_set.h>
// @@protoc_insertion_point(includes)

namespace LogonServer {

// Internal implementation detail -- do not call these.
void  protobuf_AddDesc_GameLogo_2eproto();
void protobuf_AssignDesc_GameLogo_2eproto();
void protobuf_ShutdownFile_GameLogo_2eproto();

class PROTO_CESHI;

// ===================================================================

class PROTO_CESHI : public ::google::protobuf::MessageLite {
 public:
  PROTO_CESHI();
  virtual ~PROTO_CESHI();

  PROTO_CESHI(const PROTO_CESHI& from);

  inline PROTO_CESHI& operator=(const PROTO_CESHI& from) {
    CopyFrom(from);
    return *this;
  }

  inline const ::std::string& unknown_fields() const {
    return _unknown_fields_;
  }

  inline ::std::string* mutable_unknown_fields() {
    return &_unknown_fields_;
  }

  static const PROTO_CESHI& default_instance();

  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  // Returns the internal default instance pointer. This function can
  // return NULL thus should not be used by the user. This is intended
  // for Protobuf internal code. Please use default_instance() declared
  // above instead.
  static inline const PROTO_CESHI* internal_default_instance() {
    return default_instance_;
  }
  #endif

  void Swap(PROTO_CESHI* other);

  // implements Message ----------------------------------------------

  PROTO_CESHI* New() const;
  void CheckTypeAndMergeFrom(const ::google::protobuf::MessageLite& from);
  void CopyFrom(const PROTO_CESHI& from);
  void MergeFrom(const PROTO_CESHI& from);
  void Clear();
  bool IsInitialized() const;

  int ByteSize() const;
  bool MergePartialFromCodedStream(
      ::google::protobuf::io::CodedInputStream* input);
  void SerializeWithCachedSizes(
      ::google::protobuf::io::CodedOutputStream* output) const;
  void DiscardUnknownFields();
  int GetCachedSize() const { return _cached_size_; }
  private:
  void SharedCtor();
  void SharedDtor();
  void SetCachedSize(int size) const;
  public:
  ::std::string GetTypeName() const;

  // nested types ----------------------------------------------------

  // accessors -------------------------------------------------------

  // required int64 bUserID = 1;
  inline bool has_buserid() const;
  inline void clear_buserid();
  static const int kBUserIDFieldNumber = 1;
  inline ::google::protobuf::int64 buserid() const;
  inline void set_buserid(::google::protobuf::int64 value);

  // required int32 cbPlayStatus = 2;
  inline bool has_cbplaystatus() const;
  inline void clear_cbplaystatus();
  static const int kCbPlayStatusFieldNumber = 2;
  inline ::google::protobuf::int32 cbplaystatus() const;
  inline void set_cbplaystatus(::google::protobuf::int32 value);

  // required int32 cbMaxCall = 3;
  inline bool has_cbmaxcall() const;
  inline void clear_cbmaxcall();
  static const int kCbMaxCallFieldNumber = 3;
  inline ::google::protobuf::int32 cbmaxcall() const;
  inline void set_cbmaxcall(::google::protobuf::int32 value);

  // @@protoc_insertion_point(class_scope:LogonServer.PROTO_CESHI)
 private:
  inline void set_has_buserid();
  inline void clear_has_buserid();
  inline void set_has_cbplaystatus();
  inline void clear_has_cbplaystatus();
  inline void set_has_cbmaxcall();
  inline void clear_has_cbmaxcall();

  ::std::string _unknown_fields_;

  ::google::protobuf::uint32 _has_bits_[1];
  mutable int _cached_size_;
  ::google::protobuf::int64 buserid_;
  ::google::protobuf::int32 cbplaystatus_;
  ::google::protobuf::int32 cbmaxcall_;
  #ifdef GOOGLE_PROTOBUF_NO_STATIC_INITIALIZER
  friend void  protobuf_AddDesc_GameLogo_2eproto_impl();
  #else
  friend void  protobuf_AddDesc_GameLogo_2eproto();
  #endif
  friend void protobuf_AssignDesc_GameLogo_2eproto();
  friend void protobuf_ShutdownFile_GameLogo_2eproto();

  void InitAsDefaultInstance();
  static PROTO_CESHI* default_instance_;
};
// ===================================================================


// ===================================================================

// PROTO_CESHI

// required int64 bUserID = 1;
inline bool PROTO_CESHI::has_buserid() const {
  return (_has_bits_[0] & 0x00000001u) != 0;
}
inline void PROTO_CESHI::set_has_buserid() {
  _has_bits_[0] |= 0x00000001u;
}
inline void PROTO_CESHI::clear_has_buserid() {
  _has_bits_[0] &= ~0x00000001u;
}
inline void PROTO_CESHI::clear_buserid() {
  buserid_ = GOOGLE_LONGLONG(0);
  clear_has_buserid();
}
inline ::google::protobuf::int64 PROTO_CESHI::buserid() const {
  // @@protoc_insertion_point(field_get:LogonServer.PROTO_CESHI.bUserID)
  return buserid_;
}
inline void PROTO_CESHI::set_buserid(::google::protobuf::int64 value) {
  set_has_buserid();
  buserid_ = value;
  // @@protoc_insertion_point(field_set:LogonServer.PROTO_CESHI.bUserID)
}

// required int32 cbPlayStatus = 2;
inline bool PROTO_CESHI::has_cbplaystatus() const {
  return (_has_bits_[0] & 0x00000002u) != 0;
}
inline void PROTO_CESHI::set_has_cbplaystatus() {
  _has_bits_[0] |= 0x00000002u;
}
inline void PROTO_CESHI::clear_has_cbplaystatus() {
  _has_bits_[0] &= ~0x00000002u;
}
inline void PROTO_CESHI::clear_cbplaystatus() {
  cbplaystatus_ = 0;
  clear_has_cbplaystatus();
}
inline ::google::protobuf::int32 PROTO_CESHI::cbplaystatus() const {
  // @@protoc_insertion_point(field_get:LogonServer.PROTO_CESHI.cbPlayStatus)
  return cbplaystatus_;
}
inline void PROTO_CESHI::set_cbplaystatus(::google::protobuf::int32 value) {
  set_has_cbplaystatus();
  cbplaystatus_ = value;
  // @@protoc_insertion_point(field_set:LogonServer.PROTO_CESHI.cbPlayStatus)
}

// required int32 cbMaxCall = 3;
inline bool PROTO_CESHI::has_cbmaxcall() const {
  return (_has_bits_[0] & 0x00000004u) != 0;
}
inline void PROTO_CESHI::set_has_cbmaxcall() {
  _has_bits_[0] |= 0x00000004u;
}
inline void PROTO_CESHI::clear_has_cbmaxcall() {
  _has_bits_[0] &= ~0x00000004u;
}
inline void PROTO_CESHI::clear_cbmaxcall() {
  cbmaxcall_ = 0;
  clear_has_cbmaxcall();
}
inline ::google::protobuf::int32 PROTO_CESHI::cbmaxcall() const {
  // @@protoc_insertion_point(field_get:LogonServer.PROTO_CESHI.cbMaxCall)
  return cbmaxcall_;
}
inline void PROTO_CESHI::set_cbmaxcall(::google::protobuf::int32 value) {
  set_has_cbmaxcall();
  cbmaxcall_ = value;
  // @@protoc_insertion_point(field_set:LogonServer.PROTO_CESHI.cbMaxCall)
}


// @@protoc_insertion_point(namespace_scope)

}  // namespace LogonServer

// @@protoc_insertion_point(global_scope)

#endif  // PROTOBUF_GameLogo_2eproto__INCLUDED