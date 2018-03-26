# spm-exhaustive-gen

The is the code deposite of a constant amortized time (CAT) algorithm of the sandpile model (SPM), as described in the article "A recursive structure of sand pile model and its applications", published here:

https://www.degruyter.com/view/j/puma.2015.25.issue-1/puma-2015-0006/puma-2015-0006.xml

The code is not commented as was only for internal use. I hereby release the code with GPLv3 license. Use (or read) at your own risk, although as a program it only eats up CPU time, other than that it is pretty harmless.

A little reminder: when the size of sandpile configurations is too large, the final count may exceed long long. Ignore it or use GMP, although if you enter such a size, the computer may never finish computing in our lifetime.

If you want to do something with the generated configurations, do it in the function void snapshot().
