#include "main.h"

void cleanup(va_list argm, buffer_t *output);
int run_printf(const char *format, va_list argm, buffer_t *output);
int _printf(const char *format, ...);

/**
 * cleanup - Peforms cleanup operations for _printf.
 * @argm: A va_list of arguments provided to _printf.
 * @output: A buffer_t struct.
 */
void cleanup(va_list argm, buffer_t *output)
{
	va_end(args);
	write(1, output->begin, output->ext);
	free_buffer(output);
}

/**
 * run_printf - Reads through the format string for _printf.
 * @fm: Character string to print - may contain directives.
 * @output: A buffer_t struct containing a buffer.
 * @argm: A va_list of arguments.
 *
 * Return: The number of characters stored to output.
 */
int run_printf(const char *fm, va_list argm, buffer_t *output)
{
	int i, wid, prec, ret = 0;
	char tmp;
	unsigned char flags, ext;
	unsigned int (*f)(va_list, buffer_t *,
			unsigned char, int, int, unsigned char);

	for (i = 0; *(format + i); i++)
	{
		ext = 0;
		if (*(format + i) == '%')
		{
			tmp = 0;
			flags = handle_flags(fm + i + 1, &tmp);
			wid = handle_width(argm, fm + i + tmp + 1, &tmp);
			prec = handle_precision(argm, fm + i + tmp + 1,
					&tmp);
			len = handle_extent(fm + i + tmp + 1, &tmp);

			f = handle_precise(fm + i + tmp + 1);
			if (f != NULL)
			{
				i += tmp + 1;
				ret += f(argm, output, flags, wid, prec, ext);
				continue;
			}
			else if (*(fm + i + tmp + 1) == '\0')
			{
				ret = -1;
				break;
			}
		}
		ret += _memcpy(output, (fm + i), 1);
		i += (ext != 0) ? 1 : 0;
	}
	cleanup(argm, output);
	return (ret);
}

/**
 * _printf - Outputs a formatted string.
 * @fm: Character string to print - may contain directives.
 *
 * Return: The number of characters printed.
 */
int _printf(const char *fm, ...)
{
	buffer_t *output;
	va_list argm;
	int ret;

	if (fm == NULL)
		return (-1);
	output = init_buffer();
	if (output == NULL)
		return (-1);

	va_begin(argm, fm);

	ret = run_printf(fm, argm, output);

	return (ret);
}
