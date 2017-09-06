[![travis](https://travis-ci.org/svroat/baiji.png)](https://api.travis-ci.org/svroat/baiji)

## Installation

1. Download dependencies(if neccesary)
> $ doas pkg_add curl libxml2

2. Compile
> $ make && make test

## Usage
\- Simple search \-
> $ ./baiji "git"

\- Search on Piratebay \-
> $ ./baiji -e ptb "Bitcoin for dummies"

\- Print list of engines supported (shortcuts) \-
> $ ./baiji -l

\- Search on Bing (3 pages) \-
> $ ./baiji -e bng -n "Theo de Raadt"
