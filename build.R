# build.R 4 jsonmatchCLI

#' Build an executable from a C++ source file with g++
#'
#' @param src Character. Filename of source.cpp.
#' @param exe Character. Filename for executable without extension.
#' @param argz Character. Vector of additional arguments to \code{g++}.
#' @return Character. Full path to the executable.
#'
#' @export
compileSRC2EXE <- function(src, exe='jm', argz=NULL, check=FALSE) {
  stopifnot(file.exists(src))
  is.win <- grepl('win', .Platform$OS.type, TRUE, TRUE)
  # check 4 g++ and R
  if (check && system2('g++', '--version', stdout=NULL, stderr=NULL) != 0L) {
    if (is.win) {
      stop('g++.exe not available\nit is included in the Rtools toolchain\n',
           'get Rtools from: https://cran.r-project.org/bin/windows/Rtools/\n',
           'and make sure C:\\Rtools\\mingw_(32)|(64)\\bin is on your PATH')
    } else {
      stop('g++ not available\n',
           'install g++ and make sure its directory is on your PATH')
    }
  }
  if (check && system2('R', '--version', stdout=NULL, stderr=NULL) != 0L) {
    warning('R executable not on PATH\nappend ', normalizePath(R.home('bin')))
  }
  # compile 2 executable
  SRC <- normalizePath(src, mustWork=FALSE)
  EXE <- normalizePath(file.path(getwd(),
                                 ifelse(is.win, paste0(exe, '.exe'), exe)),
                       mustWork=FALSE)
  GPPARGS <- paste0(ifelse(is.null(argz), '', paste0(argz, ' ', collapse=' ')),
                    '-o "', EXE, '" "', SRC, '"')
  if (system2('g++', GPPARGS) != 0L) stop('build failed')
  return(EXE)
}
