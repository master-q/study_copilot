module Main where

import Copilot.Language
import Fibonacci

main :: IO ()
main = interpret 50 [] fibSpec
