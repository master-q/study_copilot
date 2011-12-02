module Main where

import Language.Copilot hiding (even, odd)
import Copilot.Language
import Copilot.Compile.C99
import Wiimote

main :: IO ()
main = reify wiiSpec >>= compile defaultParams
