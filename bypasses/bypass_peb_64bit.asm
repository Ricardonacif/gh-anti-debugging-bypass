.model flat, c

bypass segment execute

hg_enter macro
push 033h ; swap to long mode
call $+5
add dword ptr [esp], 5
retf
endm

hg_end macro
call $+5
db 0C7h, 44h, 24h, 04h, 23h, 0, 0, 0 ; mov dword [rsp+4], 0x23
db 83h, 04h, 24h, 0Dh, 0CBh ; add dword [rsp], 0xD. RETF
endm

bypass_peb_64 proc

	hg_enter

	db 065h, 067h, 048h, 0A1h, 060h, 00, 00, 00 ; mov rax, gs:[0x60]
	mov byte ptr [eax+2], 00h  ; (overriding PEB64->IsBeingDebugged)
	hg_end
	ret
bypass_peb_64 endp

end
