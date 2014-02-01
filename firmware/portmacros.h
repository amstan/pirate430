#pragma once

#define OUT_BIT OUT
#undef OUT
#define DIR_BIT DIR
#undef DIR

//'#define %s_(NUM) P ## NUM ## %s\n#define %s(NUM) %s_(NUM)' % tuple([suffix]*4)

#define IN_(NUM) P ## NUM ## IN
#define IN(NUM) IN_(NUM)

#define OUT_(NUM) P ## NUM ## OUT
#define OUT(NUM) OUT_(NUM)

#define DIR_(NUM) P ## NUM ## DIR
#define DIR(NUM) DIR_(NUM)

#define REN_(NUM) P ## NUM ## REN
#define REN(NUM) REN_(NUM)

#define DS_(NUM) P ## NUM ## DS
#define DS(NUM) DS_(NUM)

#define SEL_(NUM) P ## NUM ## SEL
#define SEL(NUM) SEL_(NUM)

#define SEL0_(NUM) P ## NUM ## SEL0
#define SEL0(NUM) SEL0_(NUM)

#define SEL1_(NUM) P ## NUM ## SEL1
#define SEL1(NUM) SEL1_(NUM)

#define SELC_(NUM) P ## NUM ## SELC
#define SELC(NUM) SELC_(NUM)

#define IES_(NUM) P ## NUM ## IES
#define IES(NUM) IES_(NUM)

#define IE_(NUM) P ## NUM ## IE
#define IE(NUM) IE_(NUM)

#define IFG_(NUM) P ## NUM ## IFG
#define IFG(NUM) IFG_(NUM)

#define PORT_VECTOR_(NUM) PORT ## NUM ## _VECTOR
#define PORT_VECTOR(NUM) PORT_VECTOR_(NUM)
