# Prática: comunicação I2C entre Arduino Mega

Um Mega atua como **mestre (controller)** e envia um contador de 8 bits aos **escravos (targets)** a cada ciclo, com pausa de 500 ms. Todos compartilham o mesmo barramento. Cada placa recebe um firmware independente, com `int main(void)`, `init()` e `while (1)`.

- [main-master.cpp](main-master.cpp): tabela de destinos, envio com `for` e diagnóstico de erros.
- [main-slave.cpp](main-slave.cpp): endereço configurável, recepção por evento e impressão no programa principal.
- [Roteiro fornecido](../../docs/lab-guides/lab-09-i2c-protocol.pdf): material original. O arquivo foi recebido como lab 09, mas o título interno diz “Prática 7”; o roteiro também ilustra ATmega328P/Uno. Aqui o alvo é o **ATmega2560/Mega**.

## Ligações

Com as placas desligadas, interligue as linhas em paralelo:

| Sinal | Em cada Arduino Mega 2560 | Função |
| --- | --- | --- |
| SDA | D20 / PD1 | Dados |
| SCL | D21 / PD0 | Clock gerado pelo mestre |
| GND | GND | Referência comum |

Os pinos SDA/SCL do conector dedicado do Mega são as mesmas linhas de D20/D21. **A4/A5 são o mapeamento do Uno**, não deste exercício. Não conecte TX/RX para fazer I2C.

SDA e SCL precisam de resistores de pull-up para o nível lógico do barramento. Para estes Megas de 5 V, um ponto de partida em uma montagem curta é um resistor externo de 4,7 kΩ de SDA para 5 V e outro de SCL para 5 V; confirme as bordas no osciloscópio. O valor adequado depende da capacitância e da velocidade. Pull-ups de várias placas ficam em paralelo; verifique os já existentes. O core AVR habilita pull-ups internos, mas eles não substituem o dimensionamento do barramento.

Cada placa pode ser alimentada pelo próprio USB; mantenha os GNDs interligados. Não é necessário interligar os pinos de 5 V de placas alimentadas separadamente. Use fios curtos; mais placas e jumpers aumentam a capacitância.

## Endereços e sete participantes

| Ambiente PlatformIO | Papel/endereço decimal | Prefixo recebido |
| --- | --- | --- |
| `i2c-master` | Mestre | — |
| `i2c-slave` | Escravo 17 (`0x11`) | `x1 = ` |
| `i2c-slave-25` | Escravo 25 (`0x19`) | `x = ` |
| `i2c-slave-26` | Escravo 26 (`0x1A`) | `x2 = ` |

O exemplo de sala usava 25, 17 e **3**. Substituímos 3 por **26** porque `0x00..0x07` e `0x78..0x7F` são reservados na especificação. Use endereços de 7 bits não reservados (`0x08..0x77`, decimal 8..119), sem deslocá-los para incluir o bit de leitura/escrita. Cada escravo precisa de endereço único; `Wire.begin(17)` configura a placa local, sem procurar nem atribuir endereços às outras placas.

Com sete pessoas, supondo uma placa por pessoa e um mestre, seriam **seis escravos**. Basta ampliar a tabela `slaves` do mestre, por exemplo:

```cpp
static const Slave slaves[] = {
    {25, "x = "},
    {17, "x1 = "},
    {26, "x2 = "},
    {27, "x3 = "},
    {28, "x4 = "},
    {29, "x5 = "},
};
```

O `for` percorre toda a tabela automaticamente. Para cada placa adicional, crie um ambiente como `i2c-slave-25` no [platformio.ini](../../platformio.ini), herdando `env:i2c-slave` e trocando `-DI2C_SLAVE_ADDRESS=25` pelo endereço escolhido. Os novos endereços são um exemplo de configuração manual, não descoberta automática. A quantidade real de placas também depende das condições elétricas.

## Formato da mensagem

Mantivemos a ideia da aula: **prefixo de texto seguido de um único byte numérico**. Para o endereço 17 e contador 42:

```text
Payload em hexadecimal: 78 31 20 3D 20 2A
Interpretação:           x  1     =     [42]
Monitor do escravo:     x1 = 42
```

O terminador `\0` da string não é enviado. O último byte não é um caractere ASCII: `Wire.write(uint8_t(42))` envia `0x2A`; enviar o texto `"42"` exigiria dois bytes, `0x34 0x32`. Não se deve procurar `\0` ou uma quebra de linha para separar mensagens: neste exemplo, uma transação de escrita contém uma mensagem inteira, e o último byte é sempre o valor.

Os três contadores originais começavam em zero e eram incrementados juntos; um só `uint8_t` preserva esse comportamento. Todos os destinos recebem o mesmo valor no ciclo. Depois de 255, ele volta a 0. Para contadores independentes, adicione um campo de valor a cada elemento da tabela.

O prefixo e o valor juntos devem caber nos **32 bytes do buffer Wire no AVR**; mantenha no máximo 31 bytes de prefixo. As mensagens atuais ocupam 5 ou 6 bytes. Esse limite é da implementação, não do protocolo I2C em geral.

## Compilar, gravar e observar

A `Wire` já acompanha o framework Arduino AVR; não é preciso adicionar `lib_deps`. A extensão `.cpp` é necessária porque `Wire` e `Serial` são objetos C++. Usar `main()` não muda a linguagem para C.

```sh
pio run -e i2c-master -e i2c-slave -e i2c-slave-25 -e i2c-slave-26
pio device list
```

Grave **um ambiente por placa**, conferindo qual porta pertence a cada uma. Exemplos de portas, que devem ser substituídos pelas reais:

```sh
pio run -e i2c-slave -t upload --upload-port /dev/ttyACM0
pio run -e i2c-slave-25 -t upload --upload-port /dev/ttyACM1
pio run -e i2c-slave-26 -t upload --upload-port /dev/ttyACM2
pio run -e i2c-master -t upload --upload-port /dev/ttyACM3
pio device monitor --port /dev/ttyACM0 -b 9600
```

No escravo 17, espere `x1 = 0`, `x1 = 1` etc. O valor inicial observado pode ser maior se o mestre já estiver executando. Abrir o monitor pode reiniciar a placa. Os **9600 baud** configuram a UART do monitor; o I2C está configurado separadamente para **100 kHz**.

O mestre imprime erros de `endTransmission()` no próprio monitor:

| Código | Significado no Wire AVR |
| --- | --- |
| 0 | Transação concluída sem erro reportado |
| 1 | Dados maiores que o buffer |
| 2 | NACK no endereço: nenhum dispositivo confirmou esse destino |
| 3 | NACK em um byte de dados |
| 4 | Outro erro de barramento |
| 5 | Timeout |

O timeout de 25 ms com reinicialização do periférico evita certas esperas indefinidas; não corrige uma ligação que mantém SDA/SCL presas. O mestre segue para os outros destinos e incrementa o contador mesmo quando um envio falha. Não há retransmissão automática nem confirmação de processamento pela aplicação.

## Recepção e interrupções

`Wire.onReceive(receive_event)` registra a função; não a executa naquele momento. No core AVR, ela será chamada dentro da interrupção TWI quando a recepção terminar. Por isso ela apenas lê os bytes e os guarda. `Wire.available()` informa quantos bytes restam no buffer de software; não mede o nível elétrico de SDA.

O `main` copia a mensagem com interrupções temporariamente desabilitadas por `ATOMIC_BLOCK`, depois imprime com interrupções habilitadas. `volatile` torna os acessos compartilhados visíveis ao compilador, mas sozinho não garante uma cópia consistente de vários bytes.

O armazenamento guarda **somente a mensagem mais recente**. Se outra chegar antes da cópia, substitui a anterior. Isso é suficiente para observar um contador a cada 500 ms; uma aplicação que precisa preservar todos os comandos deve usar uma fila e definir o tratamento de fila cheia. Mensagens menores que dois bytes são descartadas. Não há validação semântica do prefixo nem CRC da aplicação.

## Conferência na bancada

1. Confirme SDA/D20, SCL/D21, GND comum, pull-ups e endereços diferentes.
2. Observe os três monitores e a volta de 255 para 0 (aproximadamente 128 segundos por ciclo completo).
3. Com as placas desligadas, remova um escravo e religue: o mestre deve reportar erro 2 para aquele endereço e continuar enviando aos outros.
4. No osciloscópio/decodificador, observe START, endereço + escrita, ACKs, payload e STOP; configure o decodificador para endereços de 7 bits.
5. Para seis escravos, amplie a tabela e grave os novos endereços; confira novamente a qualidade dos sinais.

A experiência original foi realizada em sala. A versão reorganizada precisa de validação nas placas; compilação não confirma funcionamento elétrico. Consulte o [registro de validação](../../docs/validation.md).

## Referências

- [Wire no Arduino](https://docs.arduino.cc/language-reference/en/functions/communication/wire/).
- [Wire.cpp do core AVR 1.8.6](https://github.com/arduino/ArduinoCore-avr/blob/1.8.6/libraries/Wire/src/Wire.cpp): buffers, envio e callbacks.
- [twi.c do core AVR 1.8.6](https://github.com/arduino/ArduinoCore-avr/blob/1.8.6/libraries/Wire/src/utility/twi.c): interrupção e timeout.
- [Especificação I2C da NXP, UM10204](https://www.nxp.com/docs/en/user-guide/UM10204.pdf): transações, endereços reservados e requisitos elétricos.
- [Arduino Mega 2560](https://docs.arduino.cc/hardware/mega-2560/): documentação e pinout oficial.
