begin
READ ( NUM1 ) ;

-- SUMA DE DOS NUMEROS SEGUIDOS
RESULTADO := NUM1 + NUM1 ;

-- LLAMADAS A SI MISMO
RESULTADO1 := RESULTADO + RESULTADO ;

-- CONSTAND FOLDING
RESULTADO2 := 300 + 500 + RESULTADO ;

-- ESCRITURA EN UNA "LISTA"
WRITE ( RESULTADO ,  RESULTADO1 , RESULTADO2 ) ; 


end
