section .data
    x dq 0.0
    three dq 3.0
    handler dq 0.0
section .text
global f1
f1:
    push ebp
    mov ebp, esp
        
    finit
    fld qword[ebp+8]
    fld qword[ebp+8]
    fisttp qword[x]
    fild qword[x]
    fsubp 
    f2xm1
    fld1
    faddp

    fstp qword[handler]
    
    fld qword[ebp+8]
    fisttp qword[x]
    fild qword[x]
    fld1
    fscale
    ffree st1
    fld qword[handler]
    fmulp
    fld1
    faddp
    leave
    ret

        
global f2
f2:
    push ebp
    mov ebp, esp  
    finit
    fld qword[ebp+8]
    fld qword[ebp+8]
    fld qword[ebp+8]
    fld qword[ebp+8]
    fld qword[ebp+8]
    fmulp
    fmulp
    fmulp
    fmulp
    leave
    ret  
    
    
global f3
f3:
    push ebp
    mov ebp, esp
    finit
    fld1
    fld qword[ebp+8]
    fsubp
    fld qword[three]
    fdivp 
    leave
    ret
