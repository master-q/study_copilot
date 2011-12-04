{-# LANGUAGE RebindableSyntax #-}
module Fibonacci where

import qualified Prelude as P
import Copilot.Language.Prelude
import Copilot.Language

fib :: Stream Word64
fib = [0, 1] ++ fib + drop 1 fib

fibSpec :: Spec
fibSpec = do
  trigger "fib_out" true [arg fib]
