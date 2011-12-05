{-# LANGUAGE RebindableSyntax #-}
module Main where

import qualified Prelude as P
import Language.Copilot hiding (even, odd)
import Copilot.Compile.C99

counter :: Stream Bool -> Stream Word64
counter reset = y
  where zy = [0] ++ y
        y  = if reset then 0 else zy + 1

lGetChar :: Stream Int32
lGetChar = externFun "l_getchar" []

lPrintable :: Stream Bool
lPrintable = (lGetChar >= 32) && (lGetChar < 127)

resetCounter :: Stream Bool
resetCounter = lGetChar == 114

menuSpec :: Spec
menuSpec = do
  trigger "l_putchar" lPrintable [arg lGetChar]
  trigger "counter_out" true [arg $ counter resetCounter]

main :: IO ()
main = reify menuSpec >>= compile defaultParams
