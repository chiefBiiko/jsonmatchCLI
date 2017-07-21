cmdArgs <- commandArgs(trailingOnly=TRUE)

json <- trimws(cmdArgs[1L])
pattern <- trimws(cmdArgs[2L])
auto_unbox <- if (!is.na(cmdArgs[3L])) as.logical(cmdArgs[3L]) else FALSE
strict <- if (!is.na(cmdArgs[4L])) as.logical(cmdArgs[4L]) else TRUE

jsonmatch::jsonmatch(json, pattern, auto_unbox, strict)