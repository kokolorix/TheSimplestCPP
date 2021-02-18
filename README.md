# TheSimplestCPP
A simple c++ project to play around with language and doxygen features 

[Code-Documentation](https://kokolorix.github.io/TheSimplestCPP/)

To clone all stuff do this:

`attrib +r .`

`git clone --branch features/objs --recurse-submodules https://github.com/kokolorix/TheSimplestCPP.git .`

To update the pages to the newest commit, do this:

`git submodule update --remote --merge`

`git commit -m "update to latest doc"`

To push all stuff to remote, do this:
`cd pages`

`git checkout gh-pages`

`..\generate.doc`

`git add .`

`git commit -m "update doc"`

`git push --recurse-submodules=on-demand`
