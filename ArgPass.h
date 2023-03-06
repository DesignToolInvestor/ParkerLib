/*
  A r g P a s s . h

  The ArgPassT enumeration is used when passing an argument to indicate who will own the object
  after the call.  This is primary of importance in constructors when calling methods of objects,
  where the object may retain references to the object after the method call returns until the 
  object is deleted.

  C++ 20 provides some new ways of dealing with the ownership issues.  It is likely that usage of
  this enumeration should be revisited on a case by case basis
  
    byRef: indicates that the caller will retain ownership of the object.  In this case the caller 
      is promising to retain the object as long as the callee needs it, but that it will ultimately
      assume responsibility for deleting the object.  The callee should not delete the object.

    byCopy:  indicates that the caller should make a copy of the object.  The caller may delete its
      copy of the object anytime after the caller returns.  The callee is responsible for deleting
      its copy of the object when it no longer needs it.

    byTransfer:  indicates that ownership of the object should transfer form the caller to the 
      callee.  The caller will not delete the object.  The callee is responsible for deleting the
      object when it no longer needs it.
*/

#pragma once

typedef enum {byRef, byCopy, byTransfer} ArgPassT;