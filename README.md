## Todo list

Impleting the following lexing properties
```
{- Simple type alis -}
type String = [char]

{- Algebraic data types -}
type List : #0 -> #0
    where
        Nil : List a
        Cons : a -> List a -> List a

type Lexer : #0 -> #0
    where
        Lexer : (String -> Result (s, String) Eyre) -> Lexer s

type Result : #0 -> #0 -> #0
    where
        Err : e -> Result a e
        Ok : a -> Result a e

{-
    Below is an example of a pragma
-}
{: infix(right, 0) :}
($) = \f x => f x
```
