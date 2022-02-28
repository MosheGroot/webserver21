#pragma once

namespace WS { namespace Http {

  enum StatusCode
  {
    Continue = 100,
    Processing = 102,
    Ok = 200,
    Created = 201,
    Accepted = 202,
    NoContent = 204,
    MovedPermanently = 301,
    BadRequest = 400,
    Forbidden = 403,
    NotFound = 404,
    MethodNotAllowed = 405,
    RequestTimeout = 408,
    LengthRequired = 411,
    PayloadTooLarge = 413,
    InternalServerError = 500,
    NotImplemented = 501
  };

}} //!namespace WS::Core