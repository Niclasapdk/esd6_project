function x = ringbuf_idx(start_offset, i, N)
x = mod(start_offset+i-1, N)+1; %in c: (start_offset+i)%N
end