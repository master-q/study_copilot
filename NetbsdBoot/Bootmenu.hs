{-# LANGUAGE RebindableSyntax #-}
module Main where

import qualified Prelude as P
import Language.Copilot hiding (even, odd)
import Copilot.Compile.C99

fib :: Stream Word64
fib = [0, 1] ++ fib + drop 1 fib

lGetChar :: Stream Int32
lGetChar = externFun "l_getchar" []

lPrintable :: Stream Bool
lPrintable = (lGetChar >= 32) && (lGetChar < 177)

menuSpec :: Spec
menuSpec = do
  trigger "l_putchar" lPrintable [arg lGetChar]

main :: IO ()
main = reify menuSpec >>= compile defaultParams
