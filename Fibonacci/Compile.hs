module Main where

import Language.Copilot hiding (even, odd)
import Copilot.Language
import Copilot.Compile.C99
import Fibonacci

main :: IO ()
main = reify fibSpec >>= compile defaultParams
