#define F_CPU 16000000
#define RS 1
#define E  0

#include <avr/io.h>
#include <util/delay.h>

enum{ LCD_CLEAR     = 1,
	RETURN_HOME   = 2,
	DISPLAY_ON    = 12,
	DISPLAY_OFF   = 8,
	CURSOR_ON     = 10,
	CURSOR_OFF    = 8,
	CURSOR_BLK_ON = 9,
CURSOR_BLK_OFF= 8};

char num_text [4];
char button[2] = {219, '\0'}; //165, 219, 255
char back[2] = {127, '\0'};
char left[2] = {60, '\0'};
char right[2] = {62, '\0'};
unsigned char status = 1;
unsigned char numLeds = 38;
unsigned char menu = 0, submenu = 0;
unsigned char r = 0, g = 0, b = 0;
unsigned char strips = 1;


int main() {

	DDRD = DDRD | ((1<<PD2) + (1<<PD3)); //2 n' 3: Output
	DDRC = DDRC & ~((1<<PC0) + (1<<PC1) + (1<<PC2) + (1<<PC3) + (1<<PC4)); //A0 ... A4: Input
	Lcd_init();

	PORTC = PORTC | ((1<<PC0) + (1<<PC1) + (1<<PC2) + (1<<PC3)); //A0 ... A3: Pull Up

	ADMUX |= ((1<<REFS0) + (1<<ADLAR)); //Select Vref = AVcc e 8 bits
	ADCSRA |= ((1<<ADPS2) + (1<<ADPS1) + (1<<ADPS0) + (1<<ADEN)); //Set prescaller to 128 and enable ADC

	for (unsigned char i = 0; i < numLeds; i++)
	color(0, 0, 0, 2);

	Lcd_out(1, 2, "Bastao");
	Lcd_out(2, 2, "de LED");
	_delay_ms(1000);
	while (1) {
		switch (menu) {
			case 0:
			Lcd_blink("Select", "Mode");

			Lcd_cmd(LCD_CLEAR);
			Lcd_out(1, 1, "Aut");
			Lcd_out(1, 5, "Man");
			Lcd_out(2, 1, "1 L");
			Lcd_out(2, 5, "2 L");
			_delay_ms(500);

			while (menu == 0) {
				if (~PINC & (1<<PC0)) {
					menu = 1;
					submenu = 0;
					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
				}
				else if (~PINC & (1<<PC1)) {
					menu = 2;
					submenu = 0;
					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
				}
				else if (~PINC & (1<<PC2)) {
					strips = 1;

					for (unsigned char i = 0; i < numLeds; i++) {

						for (unsigned char j = 0; j <= i; j++)
						color(0, 0, 0, 2);
						reset();
						_delay_ms(10);
						status = 0;

					}

					for (unsigned char i = 0; i < numLeds; i++)
					color(0, 0, 0, 2);

					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
					break;
				}
				else if (~PINC & (1<<PC3)) {
					strips = 2;

					for (unsigned char i = 0; i < numLeds; i++) {

						for (unsigned char j = 0; j <= i; j++)
						color(r, g, b, 2);
						reset();
						_delay_ms(10);
						status = 0;

					}

					for (unsigned char i = 0; i < numLeds; i++)
					color(r, g, b, 2);

					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
					break;
				}
			}
			break;

			case 1:
			Lcd_blink("Select", "Color");

			while (menu == 1) {
				num_text[0] = submenu + 49;
				num_text[1] = '\0';

				Lcd_out(1, 1, "Color");
				Lcd_out(1, 6, num_text);
				Lcd_out(1, 7, "/8");

				if (~PINC & (1<<PC0) && submenu > 0) {
					submenu -= 1;
					status = 1;
					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
				}
				else if (~PINC & (1<<PC1) && submenu < 7) {
					submenu += 1;
					status = 1;
					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
				}
				else if (~PINC & (1<<PC3) && submenu < 7) {
					menu = 0;
					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
				}

				switch (submenu) {
					case 0:
					Lcd_out(2, 2, "White");
					Lcd_out(2, 8, right);

					r = 255;
					g = 255;
					b = 255;

					break;
					case 1:
					Lcd_out(2, 1, left);
					Lcd_out(2, 3, "Red");
					Lcd_out(2, 8, right);

					r = 220;
					g = 0;
					b = 0;

					break;
					case 2:
					Lcd_out(2, 1, left);
					Lcd_out(2, 2, "Green");
					Lcd_out(2, 8, right);

					r = 0;
					g = 220;
					b = 0;

					break;
					case 3:
					Lcd_out(2, 1, left);
					Lcd_out(2, 3, "Blue");
					Lcd_out(2, 8, right);

					r = 0;
					g = 0;
					b = 220;

					break;
					case 4:
					Lcd_out(2, 1, left);
					Lcd_out(2, 2, "Yellow");
					Lcd_out(2, 8, right);

					r = 220;
					g = 220;
					b = 0;

					break;
					case 5:
					Lcd_out(2, 1, left);
					Lcd_out(2, 2, "Purple");
					Lcd_out(2, 8, right);

					r = 140;
					g = 0;
					b = 255;

					break;
					case 6:
					Lcd_out(2, 1, left);
					Lcd_out(2, 2, "Orange");
					Lcd_out(2, 8, right);

					r = 255;
					g = 160;
					b = 20;

					break;
					case 7:
					Lcd_out(2, 1, left);
					Lcd_out(2, 3, "Pink");

					r = 255;
					g = 0;
					b = 190;

					break;
				}

				if (status == 1) {
					for (unsigned char i = 0; i < numLeds; i++) {
						for (unsigned char j = 0; j <= i; j++)
						color(r, g, b, 3);
						if (strips == 2)
						for (unsigned char j = 0; j <= i; j++)
						color(r, g, b, 2);
						reset();
						_delay_ms(10);
						status = 0;

					}
				}

				for (unsigned char i = 0; i < numLeds; i++)
				color(r, g, b, 3);
				if (strips == 2)
				for (unsigned char i = 0; i < numLeds; i++)
				color(r, g, b, 2);
			}
			break;

			case 2:
			switch (submenu) {
				case 0:
				Lcd_out(1, 1, button);
				Lcd_out(1, 3, button);
				Lcd_out(1, 5, button);
				Lcd_out(1, 7, button);
				Lcd_out(2, 1, "R G B");
				Lcd_out(2, 7, back);

				if (~PINC & (1<<PC0)) {
					submenu = 1;
					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
				}
				else if (~PINC & (1<<PC1)) {
					submenu = 2;
					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
				}
				else if (~PINC & (1<<PC2)) {
					submenu = 3;
					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
				}
				else if (~PINC & (1<<PC3)) {
					menu = 0;
					Lcd_cmd(LCD_CLEAR);
					_delay_ms(100);
				}
				break;
				case 1:
				itoa(r, num_text, 10);
				Lcd_out(1, 3, "Red");
				Lcd_out(2, 1, left);
				Lcd_out(2, 3, "0");
				if (r < 10) {
					Lcd_out(2, 4, "00");
					Lcd_out(2, 6, num_text);
				}
				else if (r < 100) {
					Lcd_out(2, 4, "0");
					Lcd_out(2, 5, num_text);
				}
				else
				Lcd_out(2, 4, num_text);
				Lcd_out(2, 8, right);

				ADMUX = (ADMUX & 0xF0) | (PC4 & 0x0F);
				ADCSRA |= (1<<ADSC);
				while(ADCSRA & (1<<ADSC));
				r = ADCH;

				for (unsigned char i = 0; i < numLeds; i++)
				color(r, g, b, 3);
				if (strips == 2)
				for (unsigned char i = 0; i < numLeds; i++)
				color(r, g, b, 2);

				if (~PINC & (1<<PC3)) {
					menu = 2;
					submenu = 0;
					_delay_ms(100);
					Lcd_cmd(LCD_CLEAR);
				}
				break;
				case 2:
				itoa(g, num_text, 10);
				Lcd_out(1, 2, "Green");
				Lcd_out(2, 1, left);
				Lcd_out(2, 3, "0");
				if (g < 10) {
					Lcd_out(2, 4, "00");
					Lcd_out(2, 6, num_text);
				}
				else if (g < 100) {
					Lcd_out(2, 4, "0");
					Lcd_out(2, 5, num_text);
				}
				else
				Lcd_out(2, 4, num_text);
				Lcd_out(2, 8, right);

				ADMUX = (ADMUX & 0xF0) | (PC4 & 0x0F);
				ADCSRA |= (1<<ADSC);
				while(ADCSRA & (1<<ADSC));
				g = ADCH;

				for (unsigned char i = 0; i < numLeds; i++)
				color(r, g, b, 3);
				if (strips == 2)
				for (unsigned char i = 0; i < numLeds; i++)
				color(r, g, b, 2);

				if (~PINC & (1<<PC3)) {
					menu = 2;
					submenu = 0;
					_delay_ms(100);
					Lcd_cmd(LCD_CLEAR);
				}
				break;
				case 3:
				itoa(b, num_text, 10);
				Lcd_out(1, 3, "Blue");
				Lcd_out(2, 1, left);
				Lcd_out(2, 3, "0");
				if (b < 10) {
					Lcd_out(2, 4, "00");
					Lcd_out(2, 6, num_text);
				}
				else if (b < 100) {
					Lcd_out(2, 5, "0");
					Lcd_out(2, 5, num_text);
				}
				else
				Lcd_out(2, 4, num_text);
				Lcd_out(2, 8, right);

				ADMUX = (ADMUX & 0xF0) | (PC4 & 0x0F);
				ADCSRA |= (1<<ADSC);
				while(ADCSRA & (1<<ADSC));
				b = ADCH;

				for (unsigned char i = 0; i < numLeds; i++)
				color(r, g, b, 3);
				if (strips == 2)
				for (unsigned char i = 0; i < numLeds; i++)
				color(r, g, b, 2);

				if (~PINC & (1<<PC3)) {
					menu = 2;
					submenu = 0;
					_delay_ms(100);
					Lcd_cmd(LCD_CLEAR);
				}
			}
			break;
		}
	}
}


void color(unsigned char r, unsigned char g, unsigned char b, unsigned char pin) {
	for (int i = 7; i >= 0; i--) {
		int k = g >> i;
		if (k & 1)
		one(pin);
		else
		zero(pin);
	}
	
	for (int i = 7; i >= 0; i--) {
		int k = r >> i;
		if (k & 1)
		one(pin);
		else
		zero(pin);
	}
	
	for (int i = 7; i >= 0; i--) {
		int k = b >> i;
		if (k & 1)
		one(pin);
		else
		zero(pin);
	}
}


void one(unsigned char pin) {

	if (pin == 2) {
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
	}
	else if (pin == 3) {
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
	}

	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;

}

void zero(unsigned char pin) {

	if (pin == 2) {
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
		PORTD = 0b00000100;
	}
	else if (pin == 3) {
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
		PORTD = 0b00001000;
	}
	
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;
	PORTD = 0b00000000;

}


void reset() {
	
	PORTD = PORTD & ~((1<<PD2) + (1<<PD3));
	_delay_us(50);

}


void  pulso_E() {

	PORTB &= ~(1<<E);
	PORTB |= (1<<E);
	PORTB &= ~(1<<E);

}


void envia_dados(unsigned char comando) {

	PORTD = ((comando & 0xF0) | (PORTD & 0x0F));
	pulso_E();
	
	PORTD = (((comando << 4) & 0xF0) | (PORTD & 0x0F));
	pulso_E();
	
}


void Lcd_cmd(unsigned char comando) {

	_delay_ms(1);               // Temporizar 1ms
	PORTB &= ~(1<<RS);          // RS = 0
	envia_dados(comando);

}


void Lcd_out(char linha_lcd, char coluna_lcd, char *ponteiro) {

	//Seta endereço da DDRAM
	if((linha_lcd > 0) && (linha_lcd < 3))  // verifica se a linha é valida
	if((coluna_lcd > 0) && (coluna_lcd < 41))  //verifica se a coluna é valida
	{
		Lcd_cmd(128 + (coluna_lcd - 1) + ((linha_lcd - 1) * 64));
		//Envia o texto
		while (*ponteiro)
		{
			_delay_ms(1);               // Temporizar 1ms
			PORTB |= (1<<RS);           // RS = 1
			envia_dados(*ponteiro);
			ponteiro++;
		}
	}

}


void  Lcd_init() {

	//configura portas de saida
	DDRB = DDRB | ((1<<E) + (1<<RS));   // PB1 e PB0 como saida (RS/E)
	DDRD = DDRD | ((1<<PD4)+(1<<PD5)+(1<<PD6)+(1<<PD7));  // PD7 a PD4 Como saida (D7...D4)
	
	
	//inicialiaza o lcd
	PORTB &= ~(1<<RS);                                   // RS = 0
	_delay_ms(15);                                       // Temporizar 15 ms
	PORTD = ((0x30 & 0xF0) | (PORTD & 0x0F));            // Enviar 0x30 ao LCD
	pulso_E();                                           // Pulso no pino E
	
	_delay_ms(5);                                        // Temporizar 5 ms
	pulso_E();                                           // Pulso no pino E
	
	_delay_ms(1);                                        // Temporizar 1 ms
	pulso_E();                                           // Pulso no pino E
	
	_delay_ms(1);                                        // Temporizar 1 ms
	PORTD = ((0x20 & 0xF0) | (PORTD & 0x0F));            // Enviar 0x20 ao LCD
	pulso_E();                                           // Pulso no pino E
	
	Lcd_cmd(0x28);                                      //Especifica a interface de comunicação 4 bits, 2 linhas 5X8 pixels
	Lcd_cmd(0x08);                                      // Desliga o display
	Lcd_cmd(0x01);                                      // Limpa o diplay
	Lcd_cmd(0x06);                                      // Não desloca o display, incrementa o AC (posição do cursor)
	Lcd_cmd(DISPLAY_ON);

}


void Lcd_blink(char *frase1, char *frase2) {

	Lcd_cmd(LCD_CLEAR);
	Lcd_out(1, 2, frase1);
	Lcd_out(2, 2, frase2);
	_delay_ms(200);
	Lcd_cmd(LCD_CLEAR);
	_delay_ms(200);
	Lcd_out(1, 2, frase1);
	Lcd_out(2, 2, frase2);
	_delay_ms(200);
	Lcd_cmd(LCD_CLEAR);
	_delay_ms(200);
	Lcd_out(1, 2, frase1);
	Lcd_out(2, 2, frase2);
	_delay_ms(200);
	Lcd_cmd(LCD_CLEAR);

}