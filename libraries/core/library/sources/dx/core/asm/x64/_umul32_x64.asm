_DATA SEGMENT
_DATA ENDS

_TEXT SEGMENT

; https://learn.microsoft.com/en-us/cpp/build/x64-calling-convention?view=msvc-170
; https://www.felixcloutier.com/x86/mul
; uint16_t _dx_umul32_x64(uint16_t x, uint16_t y, uint16_t *z).
_dx_umul32_x64 PROC
  ; The stack has the following layout:
  ; -------------- ESP + 0
  ; return address
  ; -------------- ESP + 4

  ; Perform multiplication ecx * edx such that
  ; - the lower word of the product is in eax
  ; - the higher word in edx
  ; - overflow and carry flags are 1 if an overflow occurs, otherwise they are 0.
  MOV ax, cx                    ; ax@1 := cx@1
  MUL dx                        ; dx@2 := hi(ax@1 * dx@1), ax@2 = lo(ax@1 * dx@1)

  ; Store the higher word in *z. z is stored in r8.
  MOV WORD PTR [r8], dx         ; *z = dx@2
  
  ; The return address is on top of the stack.
  ; "RET 0" then returns to that return address.
  ; The return value is in ax@2.
  RET 0
_dx_umul32_x64 ENDP

_TEXT ENDS

END