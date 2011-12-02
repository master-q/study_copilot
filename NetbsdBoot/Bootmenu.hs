module Main where

import qualified Prelude as P
import Language.Copilot hiding (even, odd)
import Copilot.Compile.C99

fib :: Stream Word64
fib = [0, 1] ++ fib + drop 1 fib

fibSpec :: Spec
fibSpec = do
  trigger "pout_w64" true [arg fib]

main :: IO ()
main = reify fibSpec >>= compile defaultParams
