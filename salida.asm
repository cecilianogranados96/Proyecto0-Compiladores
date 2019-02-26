section     .data	  	  
 											      
 	;-------------------------------------------- 
 	; En esta sección se declaran las variables.  
 	; Se reserva la memoria necesaria.			  
 	; Se declaran las constantes.				  
 	;-------------------------------------------  
 												  
 	global buffer_input   ; Buffer donde se almacenarán los valores.			
 	global len_input	  ; largo del buffer.									
 	global buffer_output  ; Buffer donde se escribirán los valores a imprimir.	
 	global len_output	  ; largo del buffer.									
 																				
 	buffer_input  db '*******************************', 0xa						
 	len_input      equ $ - buffer_input											
 	buffer_output db '*******************************', 0xa                     
 	len_output      equ $ - buffer_input 										
 	
  
section     .text	  	
 											 
 	;----------------------------------------------- 
 	; En esta sección se realizan las operaciones. 	
 	; Se escribe el código necesario para ejecutar.	
 	; Realiza los llamados adecuados.				
 	;---------------------------------------------- 
                                                     
     extern buffer_input  	  	  
     extern len_input			  
     extern buffer_output		  
     extern len_output			  
     global      _start  		  
                                   
     ; Funciones con etiquetas para desplazarme por el archivo						
     ; //////////////////////////////////////////////////////////////////////////////////////////////////////////////// 
 
     ; Funciones 
                                                     
     ; # Atoi = edi -> contador; esi -> largo; eax -> número_final; 
 
     atoi:						
         xor ecx, ecx   						; Limpio el buffer 
         mov cl, [buffer_input + esi]  	 						 
         sub cl, '0'							; Convierto a decimal  
         mul ebx 								; Multiplico por diez  
         add eax, ecx 						; Agrego el caracter convertido 
         inc esi 							
         cmp edi, esi 						; comparo si ya llegue al final 
         jne atoi							; siga  
         ret     							 
 
 
     itoa:  						
 	        ; Me convierte a ASCII 
 
	        xor edx, edx						; Limpio el registro edx  
 	        div ebx								; eax:dividendo y resultado, edx:residuo, ebx:divisor 
 	        add dl, '0'							; Aplique la conversion a ASCII   
 	        mov [buffer_output + esi], dl 		; Mueva a resultado en la posicion (UM, C, D, U) 
 	        dec esi								; Decremento la actual posición. 
 	        cmp esi, 0							; Si ya llegue al inicio, ponga cero. 
 	        jne itoa  							; Si no realice de nuevo	
 	                                            ; siga  
         ret 								
 
 
     _start:  	
 
 ; Code 
         xor eax, eax 
  
 
 	push eax 
	; add Literal, result 
             ; ---------------------------  
             mov eax, Temp&1        
             mov [esp + 700], ax   
             ; ---------------------------  
 
 	; mov destino, origen 
                     ; ---------------------------  
                     mov eax, [esp + 700] 
                     mov [esp + A], ax 
                     ; ---------------------------  
 	; Escribir en consola -> write 
 	; ---------------------------  
 	; 
 	mov ax, [esp + A]  ; pongo en el ax e
 	mov ebx, 10 
 	mov esi, 30 
 	call itoa   
 	
 	mov eax, 4 	; EAX -> write  
 	mov ebx, 1 	; EBX -> input  
 	mov ecx, buffer_output ; ECX -> valor que se va a imprimir     
 	mov edx, len_output	; EDX -> largo de lo que voy a escribir 
 	int 0x80 	; llamada al sistema operativo      
 	; ---------------------------  
 
 	  	; ---------------------------  
  	mov eax, 1 
 	int 0x80 
  
