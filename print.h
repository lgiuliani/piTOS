extern void print (char * s, void (*print_func) (char s));
extern void print_num(unsigned int num, void (*print_func) (char s));
extern void print_num_dec(unsigned int num, void (*print_func) (char s));
extern void print_pair(char * s, unsigned int num);
extern void print_pair_dec(char * s, unsigned int num);

/** If num != val, print 's' as an error */
extern void print_assert(char *s, unsigned int num, unsigned int val);
