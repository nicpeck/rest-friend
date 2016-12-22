# REST friends 4eva

The aim of this module is to be a starting point for creating custom RESTful APIs managed by Drupal. Many of the REST modules out there try and replicate all Drupal functionality over a REST interface, but this module is to help create more abstract/custom APIs.

## Getting started

The easiest way to get started is to make a copy of the `restfriend_example` submodule and change the name (directory name; restfriend_example.info & restfriend_example.module filenames; restfriend_example_ hook functions). It doesn't have to be in the same directory as the `restfriend_core` module, it just has to list it as a dependency.

The the example submodule defines and constructs an instance of the `$RestFriend` class, which needs to be made global in your submodule so you can add access and manipulate it.

## Registering endpoints

Define a custom endpoint by adding it to the global `$RestFriend` object:

```
$RestFriend->addEndpoint($method, $path, $callback, $access = array());
```

`string $metod`: The HTTP request method for which to make the endpoint available. POST, GET, PUT, PATCH & DELETE will be supported.  
`string $path`: The Drupal menu path to map to.  
`closure $callback`: The function to run at the given endpoint.  
`array $access` _(optional)_ : User permissions required to access the endpoint. Described in more detail in Authentication

Once defined, the endpoint paths need to be registered in Drupal's menu. That process is demonstrated in the example submodule, and can be copied directly into a new submodule.


## Authentication

You can add restrictions to endpoints with Drupal access arguments to be passed to the user_access function. If access arguments are present, the request must contain a token. These can be default access strings, defined by other modules, or defined in your implementation by invoking `hook_permission()`.

There are 2 steps to making authorised requests:

1. Get your login token sending HTTP Basic Auth header to the authenticate endpoint (Username & password should be in the format `username:password` and base64 encoded). The authentication endpoint must be defined in your custom module, but can use the default callback `restfriend_core_authenticate()`
2. Send your token in a Bearer Auth header.

```
# REQUEST
GET /api/v1/authenticate
Authorization: Basic QWxhZGRpbjpvcGVuIHNlc2FtZQ

# RESPONSE
{
  "success": true,
  "message": "Logged in successfully.",
  "token": "6NXWN6V03GG0400C0OO8CC0CG"
}

----------------

# REQUEST
GET /api/v1/resource
Authorization: Bearer 6NXWN6V03GG0400C0OO8CC0CG
```

Any endpoint defined with access arguments will try to validate the user, and rewrite the global `$user` object. If a bearer token is not sent, it will just load the anonymous user object, but if a token is sent but not valid/found, it will throw an error.
