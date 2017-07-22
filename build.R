# build.R 4 jsonmatchCLI

#' Build an executable 4 jsonmatch
#' 
#' @param SRC. Character. Filename of source: 'generic.cpp'.
#' @return Character. Full path to the \code{jm} executable.
#' 
#' @export
compileSRC2EXE <- function(SRC, ARGS) {
  stopifnot(is.character(SRC), length(SRC) == 1L, nchar(SRC) != 0L)
  is.win <- grepl('win', .Platform$OS.type, TRUE, TRUE)
  # check 4 g++ and R
  if (system2('g++', '--version', stdout=NULL, stderr=NULL) != 0L) {
    if (is.win) {
      stop('g++.exe not available\nit is included in the Rtools toolchain\n', 
           'get Rtools from: https://cran.r-project.org/bin/windows/Rtools/\n',
           'and make sure C:\\Rtools\\mingw_(32)|(64)\\bin is on your PATH')
    } else {
      stop('g++ not available\n', 
           'install g++ and make sure its directory is on your PATH')
    }
  }
  if (system2('R', '--version', stdout=NULL, stderr=NULL) != 0L) {
    warning('R executable not on PATH\nappend ', normalizePath(R.home('bin')))
  }
  # compile 2 executable
  SRC <- normalizePath(SRC)
  EXE <- normalizePath(file.path(getwd(), if (is.win) 'jm.exe' else 'jm'))
  gppargs <- paste0(ifelse(missing(ARGS), '', paste0(ARGS, ' ', collapse=' ')), 
                    '-o "', EXE, '" "', SRC, '"')
  if (system2('g++', gppargs) != 0L) stop('build failed')
  return(EXE)
}
