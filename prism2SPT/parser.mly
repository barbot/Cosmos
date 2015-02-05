%{
  open Type
  open PrismType
%}

%token <int> INT
%token <float> FLOAT
%token <string> NAME INTNAME BOOLNAME DOUBLENAME
%token <string> STRING
%token LPAR RPAR
%token PLUS MINUS MULT DIV
%token LSQBRAK RSQBRAK
%token EOL
%token SEMICOLON COLON PRIME COMMA QMARK
%token AND OR
%token NOT
%token BOOL TRUE FALSE
%token CONST
%token EQ SG SL GE LE
%token RANGE 
%token CTMC MODULE ENDMODULE INIT ENDINIT REWARDS ENDREWARDS FORMULA 
%token ARROW
%token EOF
%token INTKW DOUBLEKW
%token LABEL
%token FLOOR CEIL

%left OR
%left AND
%left NOT
%left EQ SG SL GE LE
%left PLUS MINUS
%left MULT DIV
%left LPAR RPAR

%start main
%type <PrismType.constdef*PrismType.prism_file> main
%type <int expr'> intexpr
%type <float expr'> floatexpr
%type <bool expr'> stateCondition
%%

main:
  CTMC defmod initrew EOF {($2)};

defmod:
  definition defmod { let (defi1,defd1) = $1 and ((defi2,defd2),modl) = $2 in 
		      ((defi1@defi2,defd1@defd2)  ,modl) }
  | modulelist defmod { let (defl,modl) = $2 in (defl,$1::modl) }
  | { (([],[]),[]) }
;

anyname:
  NAME {$1}
| BOOLNAME {$1}
| INTNAME {$1}
| DOUBLENAME {$1}
;

definition:
  CONST INTKW INTNAME EQ intexpr SEMICOLON { add_int $3; [($3,Some $5)] , [] }
| CONST INTNAME EQ intexpr SEMICOLON     { add_int $2; [($2,Some $4)] , [] }
| CONST DOUBLEKW DOUBLENAME EQ floatexpr SEMICOLON { add_double $3; [], [($3,Some $5)] }
| CONST INTKW INTNAME SEMICOLON  {         add_int $3; [($3,None)]    , []  }
| CONST DOUBLEKW DOUBLENAME SEMICOLON {       add_int $3; [], [($3,None)] }
| FORMULA NAME EQ intexpr SEMICOLON { [],[] }
| FORMULA NAME EQ stateCondition SEMICOLON { [],[] }
| FORMULA NAME EQ floatexpr SEMICOLON { [],[] }
| LABEL STRING EQ stateCondition SEMICOLON { [],[] }
;

modulelist:
  MODULE anyname varlist actionlist ENDMODULE {
    Full { name=$2;
      varlist=$3;
      actionlist=$4;
      actionset = find_action $4
    }
  }
  | MODULE anyname EQ anyname LSQBRAK renamelist RSQBRAK ENDMODULE {
    Renaming ($2,$4,$6)
  }
;

renamelist:
  anyname EQ anyname {[($1,$3)]}
  | anyname EQ anyname COMMA renamelist {($1,$3)::$5}
;

varlist:
  anyname COLON rangevar INIT TRUE SEMICOLON varlist 
  { ($1,$3,Int 1)::$7 }  
  | anyname COLON rangevar INIT FALSE SEMICOLON varlist 
      { ($1,$3,Int 0)::$7 }  
  | anyname COLON rangevar INIT intexpr SEMICOLON varlist 
      { ($1,$3,(simp_int $5))::$7 }
  | anyname COLON rangevar SEMICOLON varlist 
      { ($1,$3,(fst $3))::$5 }
| {[]}
;

rangevar:
  LSQBRAK intexpr RANGE intexpr RSQBRAK { ($2,$4) }
| BOOL { (Int 0,Int 1) }
;

actionlist:
  headaction stateCondition ARROW tailaction SEMICOLON actionlist
  { List.fold_left (fun l (r,u) -> ($1,$2,r,u)::l) $6 $4}
| {[]}
;

headaction:
  LSQBRAK RSQBRAK {None}
| LSQBRAK anyname RSQBRAK {Some $2}
;

tailaction:
 update {[Float(1.0),$1]}
| floatexpr COLON update {[$1,$3]}
| floatexpr COLON update PLUS tailaction {($1,$3)::$5}
; 

floatexpr:
  INT {Float (float $1)}
| FLOAT {Float($1)}
| INTNAME {CastInt(IntName $1)}
| LPAR floatexpr RPAR {$2 }
| floatexpr MULT floatexpr {Mult($1,$3)}
| floatexpr PLUS floatexpr {Plus($1,$3)}
| floatexpr MINUS floatexpr {Minus($1,$3)}
| floatexpr DIV floatexpr {Div($1,$3)}
| DOUBLENAME {FloatName($1)}
| NAME LPAR floatexprlist RPAR {FunCall($1,$3) }
| LPAR stateCondition QMARK floatexpr COLON floatexpr RPAR { If($2,$4,$6) }
;

floatexprlist:
  floatexpr COMMA floatexprlist {$1::$3}
| floatexpr { [$1] }
| { [] }
;
cmp:
  EQ {EQ}
 | SG {SG}
 | SL {SL}
 | GE {GE}
 | LE {LE}
 | NOT EQ {NEQ}
;

update:
  TRUE { [] } 
| LPAR upatom RPAR { [$2] }
| LPAR upatom RPAR AND update {$2::$5}
;

upatom:
  anyname PRIME EQ intexpr {$1,IntUp(eval $4)}
| anyname PRIME EQ stateCondition {$1,BoolUp(eval $4)}
;

initrew: 
  INIT TRUE ENDINIT initrew {()}
| REWARDS STRING actionrewardlist ENDREWARDS initrew {()}
| {()}
;


actionrewardlist:
LSQBRAK NAME RSQBRAK stateCondition COLON floatexpr SEMICOLON actionrewardlist
  {()}
| LSQBRAK RSQBRAK stateCondition COLON floatexpr SEMICOLON actionrewardlist
  {()}
|  stateCondition COLON floatexpr SEMICOLON actionrewardlist
  {()}
| {()}
;

stateCondition:
TRUE {Bool true}
| FALSE {Bool false}
| BOOLNAME { BoolName($1) }
| stateCondition AND stateCondition {And($1,$3)}
| stateCondition OR stateCondition {Or($1,$3)}
| NOT stateCondition {Not($2)}
| LPAR stateCondition RPAR {$2}
| intexpr cmp intexpr  { IntAtom(simp_int $1,$2,simp_int $3) }
| LPAR stateCondition QMARK stateCondition COLON stateCondition RPAR { If($2,$4,$6) }
;

intexpr:
  INT {Int($1)}
 | INTNAME {IntName($1)}
 | LPAR intexpr RPAR {$2}
 | intexpr PLUS intexpr {Plus($1,$3)}
 | intexpr MULT intexpr {Mult($1,$3)}
 | intexpr MINUS intexpr {Minus($1,$3)}
 | NOT intexpr {Minus(Int 1,$2)}
 | FLOOR LPAR floatexpr RPAR {Floor($3)}
 | CEIL LPAR floatexpr RPAR {Ceil($3)}
 | LPAR stateCondition QMARK intexpr COLON intexpr RPAR { If($2,$4,$6) }
;