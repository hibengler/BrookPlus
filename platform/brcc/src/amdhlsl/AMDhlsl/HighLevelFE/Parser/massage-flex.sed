# Redirects the <unistd.h> in the flex-generated scanner to something we control
# to fix win/lin portability

s/<unistd.h>/"unistd-wrapper.h"/
s/^#line.*$//
