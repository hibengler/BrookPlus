# Various tweaks to flex generated scanners to improve portability and
# better cope with version-specific weirdnesses 

# Redirects the <unistd.h> in the flex-generated scanner to something we control
# to fix win/lin portability

s/<unistd.h>/"unistd-wrapper.h"/
s/ps2arb_wrap(n) 1/ps2arb_wrap() 1/