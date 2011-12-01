
#define link_warning(symbol, msg) \
  static const char __evoke_link_warning_##symbol[] \
    __attribute__((__used__, section (".gnu.warning." #symbol "\n\t#"))) = msg
/* A canned warning for sysdeps/stub functions.
   The GNU linker prepends a "warning: " string.  */


#define stub_warning(name) \
  link_warning(name, \
  "the `" #name "\' function is not implemented and will always fail")
