https://api.travis-ci.org/svroat/baiji.png
# Installation 

1. Download dependencies:
> $ doas pkg_add curl libxml2 

2. Compile:
> $ make

# Usage
Find who is the hacker known as 4chan.
`$ ./baiji "4chan"`
Search on piratebay.
`$ ./baiji -e ptb "Twin Peaks"`
Print list of engines supported and his shortcuts.
`$ ./baiji -l`
Search on bing in 3 pages about "Theo de Raadt".
`$ ./baiji -e bng -n 3 "Theo de Raadt"`
