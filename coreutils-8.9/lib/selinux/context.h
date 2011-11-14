/* DO NOT EDIT! GENERATED AUTOMATICALLY! */
#ifndef SELINUX_CONTEXT_H
# define SELINUX_CONTEXT_H

# include <errno.h>

/* The definition of _GL_UNUSED_PARAMETER is copied here.  */
/* _GL_UNUSED_PARAMETER is a marker that can be appended to function parameter
   declarations for parameters that are not used.  This helps to reduce
   warnings, such as from GCC -Wunused-parameter.  The syntax is as follows:
       type param _GL_UNUSED_PARAMETER
   or more generally
       param_decl _GL_UNUSED_PARAMETER
   For example:
       int param _GL_UNUSED_PARAMETER
       int *(*param)(void) _GL_UNUSED_PARAMETER
   Other possible, but obscure and discouraged syntaxes:
       int _GL_UNUSED_PARAMETER *(*param)(void)
       _GL_UNUSED_PARAMETER int *(*param)(void)
 */
#ifndef _GL_UNUSED_PARAMETER
# if __GNUC__ >= 3 || (__GNUC__ == 2 && __GNUC_MINOR__ >= 7)
#  define _GL_UNUSED_PARAMETER __attribute__ ((__unused__))
# else
#  define _GL_UNUSED_PARAMETER
# endif
#endif

typedef int context_t;
static inline context_t context_new (char const *s _GL_UNUSED_PARAMETER)
  { errno = ENOTSUP; return 0; }
static inline char *context_str (context_t con _GL_UNUSED_PARAMETER)
  { errno = ENOTSUP; return (void *) 0; }
static inline void context_free (context_t c _GL_UNUSED_PARAMETER) {}

static inline int context_user_set (context_t sc _GL_UNUSED_PARAMETER,
                                    char const *s _GL_UNUSED_PARAMETER)
  { errno = ENOTSUP; return -1; }
static inline int context_role_set (context_t sc _GL_UNUSED_PARAMETER,
                                    char const *s _GL_UNUSED_PARAMETER)
  { errno = ENOTSUP; return -1; }
static inline int context_range_set (context_t sc _GL_UNUSED_PARAMETER,
                                     char const *s _GL_UNUSED_PARAMETER)
  { errno = ENOTSUP; return -1; }
static inline int context_type_set (context_t sc _GL_UNUSED_PARAMETER,
                                    char const *s _GL_UNUSED_PARAMETER)
  { errno = ENOTSUP; return -1; }

#endif
