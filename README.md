# Analisador de Sistemas Embarcados

Projeto educacional de engenharia voltado ao estudo prático de **firmware, sinais digitais, barramentos de comunicação, instrumentação e diagnóstico de sistemas embarcados** utilizando ESP32, ESP-IDF e ferramentas de bancada.

O objetivo não é criar um produto comercial. O projeto funciona como um laboratório técnico de longo prazo para compreender como o hardware e o firmware realmente se comportam, registrar evidências e desenvolver competências compatíveis com uma posição júnior em engenharia de firmware.

---

## Visão geral

O Analisador de Sistemas Embarcados será desenvolvido de forma incremental, começando pela validação do ambiente de desenvolvimento e evoluindo até a captura, interpretação e diagnóstico de diferentes protocolos digitais.

O projeto prioriza:

- compreensão elétrica e lógica dos sinais;
- desenvolvimento profissional com ESP-IDF;
- firmware modular em C;
- uso de FreeRTOS;
- análise com multímetro e analisador lógico;
- documentação técnica e rastreabilidade;
- testes planejados pelo Modelo V;
- registro diário de estudo, desenvolvimento e dificuldades;
- validação em hardware real.

A parte de software externo será mantida simples durante as primeiras fases. O foco inicial é firmware, eletrônica, comunicação e instrumentação. Interfaces mais completas serão desenvolvidas somente depois que as funções embarcadas estiverem consolidadas.

---

## Objetivos educacionais

Ao longo do projeto, pretende-se desenvolver domínio sobre:

1. Estrutura profissional de projetos ESP-IDF.
2. Organização modular de firmware em componentes.
3. Configuração de GPIOs e periféricos internos.
4. Funcionamento elétrico e lógico de barramentos digitais.
5. Temporização, clock, baud rate e sincronização.
6. Captura e interpretação de sinais.
7. Identificação de falhas de comunicação.
8. Uso de interrupções, filas, buffers e máquinas de estado.
9. Concorrência e organização de tarefas com FreeRTOS.
10. Registro de eventos e dados em memória ou microSD.
11. Criação de planos de teste e critérios de aceitação.
12. Análise de consumo de RAM, flash e tempo de execução.
13. Produção de documentação técnica e evidências de laboratório.
14. Uso de Git e GitHub para rastreabilidade do firmware.

---

## Protocolos e comunicações estudados

### UART

A UART será uma das primeiras interfaces estudadas por ser assíncrona e muito utilizada em microcontroladores, módulos GPS, conversores USB-serial, modems e interfaces de depuração.

Tópicos previstos:

- linhas TX e RX;
- referência comum de GND;
- baud rate;
- bits de dados;
- paridade;
- stop bits;
- comunicação full-duplex;
- framing error;
- overflow de buffer;
- ruído e perda de bytes;
- captura com analisador lógico;
- cálculo e detecção aproximada de baud rate;
- timestamps e registro de mensagens;
- interpretação de protocolos construídos sobre UART.

### I²C

O I²C será estudado como barramento síncrono compartilhado, comum em sensores, memórias EEPROM, RTCs, expansores de GPIO e displays.

Tópicos previstos:

- linhas SDA e SCL;
- operação open-drain;
- resistores de pull-up;
- endereçamento;
- condição de START e STOP;
- ACK e NACK;
- leitura e escrita;
- clock stretching;
- múltiplos dispositivos no mesmo barramento;
- colisões e endereços duplicados;
- efeito da capacitância do barramento;
- análise de níveis lógicos;
- diagnóstico de barramento travado.

### SPI

O SPI será estudado como interface síncrona de alta velocidade, comum em memórias, displays, conversores, módulos Ethernet e cartões microSD.

Tópicos previstos:

- MOSI, MISO, SCLK e CS;
- comunicação full-duplex;
- modos CPOL e CPHA;
- frequência de clock;
- seleção de escravos;
- ordem de bits;
- transações e enquadramento;
- múltiplos dispositivos;
- análise de temporização;
- erros de modo SPI;
- captura e decodificação com analisador lógico.

### RS-485

O RS-485 será estudado como camada física diferencial adequada a ambientes industriais e distâncias maiores.

Tópicos previstos:

- sinais diferenciais A e B;
- operação half-duplex;
- controle de direção;
- terminação;
- resistores de polarização;
- topologia de barramento;
- referência de terra;
- imunidade a ruído;
- uso de transceptores;
- diagnóstico com multímetro e analisador lógico;
- relação entre RS-485 e protocolos de camada superior.

### Modbus RTU

O Modbus RTU será utilizado para estudar um protocolo industrial sobre UART/RS-485.

Tópicos previstos:

- mestre e escravo;
- endereço do dispositivo;
- códigos de função;
- registradores;
- construção de quadros;
- CRC-16;
- tempos entre caracteres e quadros;
- exceções Modbus;
- timeout;
- retransmissão;
- validação de quadros;
- testes com mensagens válidas e corrompidas.

### CAN / TWAI

O ESP32 possui controlador TWAI, compatível com a camada de enlace do CAN clássico. Essa etapa será utilizada para compreender redes embarcadas robustas e orientadas a mensagens.

Tópicos previstos:

- barramento diferencial;
- identificadores;
- arbitragem;
- bits dominantes e recessivos;
- filtros;
- confirmação;
- detecção de erros;
- estados de erro;
- retransmissão;
- terminação de 120 ohms;
- análise de carga do barramento;
- frames de dados e frames remotos;
- integração entre controlador e transceptor CAN.

### 1-Wire

O 1-Wire poderá ser utilizado para estudar comunicação com apenas uma linha de dados, especialmente com sensores como o DS18B20.

Tópicos previstos:

- temporização;
- pulsos de reset e presença;
- slots de leitura e escrita;
- endereçamento por ROM;
- alimentação parasita;
- sensibilidade a atrasos e comprimento de fios.

### GPIO, PWM e sinais digitais

Antes dos protocolos mais complexos, serão estudados os fundamentos:

- níveis lógicos;
- entrada e saída digital;
- pull-up e pull-down;
- bordas de subida e descida;
- debounce;
- interrupções;
- geração e medição de pulsos;
- PWM;
- frequência;
- período;
- duty cycle;
- contadores;
- timestamps;
- análise de jitter.

---

## Ferramentas

### Hardware principal

- ESP32 DevKit;
- protoboard;
- jumpers;
- resistores;
- LEDs;
- botões;
- sensores e módulos de comunicação;
- conversores USB-serial;
- transceptores RS-485;
- futuros transceptores CAN;
- módulo microSD;
- fonte ou alimentação USB adequada.

### Instrumentação

- multímetro;
- analisador lógico;
- estação de solda;
- cabos e pontas de teste;
- osciloscópio não utilizado nas primeiras etapas.

O analisador lógico será utilizado para observar estados digitais, temporização e decodificação de protocolos. O multímetro será utilizado para continuidade, níveis de tensão, alimentação e verificações elétricas básicas.

### Software

- Windows 10;
- WSL 2;
- Ubuntu 26.04 LTS;
- Visual Studio Code;
- ESP-IDF 6.0.1;
- Python 3.14;
- CMake;
- Ninja;
- ccache;
- Git;
- GitHub;
- usbipd-win;
- ferramentas de monitoramento e gravação do ESP-IDF;
- software do analisador lógico.

### Toolchain validada

O primeiro ambiente foi validado com:

```text
ESP-IDF: 6.0.1
Compilador: xtensa-esp-elf-gcc 15.2.0
CMake: 4.2.3
Ninja: 1.13.2
ccache: 4.12.3
Python: 3.14.4
WSL: 2.7.8.0
```

---

## Organização do projeto

```text
ambiente_analisador/
├── CMakeLists.txt
├── sdkconfig.defaults
├── main/
│   ├── CMakeLists.txt
│   └── main.c
├── components/
├── docs/
│   ├── 00_ambiente/
│   ├── 01_objetivos_educacionais/
│   ├── 02_arquitetura/
│   ├── 03_hardware/
│   ├── 04_protocolos/
│   ├── 05_planos_de_teste/
│   ├── 06_evidencias/
│   └── 07_diario_de_laboratorio/
├── hardware/
│   ├── diagramas/
│   ├── pinagem/
│   ├── bom/
│   └── capturas_analisador_logico/
├── tools/
│   ├── scripts/
│   └── host/
└── README.md
```

### `main/`

Contém o ponto de entrada inicial do firmware. Conforme o projeto evoluir, o código será dividido em componentes específicos.

### `components/`

Será utilizado para os módulos internos do firmware, por exemplo:

```text
components/
├── signal_capture/
├── uart_analyzer/
├── i2c_analyzer/
├── spi_analyzer/
├── rs485_transport/
├── modbus_parser/
├── twai_analyzer/
├── storage/
├── diagnostics/
└── system_monitor/
```

Os nomes poderão ser ajustados conforme a implementação real.

### `docs/`

Contém objetivos, arquitetura, requisitos, planos de teste, evidências e registros diários.

### `hardware/`

Armazena diagramas, pinagens, listas de materiais e capturas realizadas com instrumentos.

### `tools/`

Reservado para scripts auxiliares e ferramentas executadas no computador, sem substituir o foco principal no firmware.

---

## Metodologia de desenvolvimento

### Projeto educacional

Este repositório não será tratado como um produto comercial. Cada etapa deve produzir aprendizado verificável.

Uma funcionalidade somente será considerada concluída quando houver:

- objetivo definido;
- base teórica estudada;
- implementação;
- teste em hardware;
- evidência;
- análise do resultado;
- registro das dificuldades;
- conclusão técnica.

### Modelo V

O Modelo V será utilizado como estrutura de estudo e validação.

Lado esquerdo:

1. objetivo educacional;
2. requisitos de aprendizagem;
3. arquitetura;
4. projeto dos módulos;
5. planejamento dos testes.

Base:

6. implementação do firmware.

Lado direito:

7. teste de unidade;
8. teste de integração;
9. teste de sistema;
10. validação do objetivo educacional.

Exemplo:

```text
Requisito:
O firmware deve transmitir bytes por UART a 115200 bit/s.

Projeto:
Configurar UART, GPIOs, buffers e tarefa de transmissão.

Teste de unidade:
Verificar a função responsável pela montagem dos dados.

Teste de integração:
Capturar TX com o analisador lógico.

Teste de sistema:
Confirmar baud rate, conteúdo e periodicidade.

Validação:
Explicar a estrutura do quadro UART e justificar a configuração.
```

---

## Controle de estudo e diário de laboratório

O acompanhamento começou em **10/07/2026**, definido como Dia 1.

Cada dia deve registrar:

- data;
- horário inicial e final;
- tempo total;
- objetivo;
- atividades;
- comandos utilizados;
- dificuldades;
- hipóteses;
- testes;
- resultados;
- evidências;
- aprendizado;
- avaliação pessoal;
- próximos passos.

As avaliações periódicas considerarão:

- compreensão;
- autonomia;
- organização;
- capacidade de diagnóstico;
- qualidade do código;
- qualidade dos testes;
- uso de instrumentos;
- clareza da documentação.

---

## Plano de desenvolvimento

### Fase 0 — Ambiente e organização

- [x] Criar a raiz do projeto.
- [x] Validar WSL 2.
- [x] Validar Ubuntu.
- [x] Validar VS Code.
- [x] Validar ESP-IDF.
- [x] Validar toolchain Xtensa.
- [x] Criar estrutura documental.
- [x] Criar projeto mínimo.
- [x] Compilar o primeiro firmware.
- [x] Gravar o firmware no ESP32.
- [x] Validar o monitor serial.
- [x] Configurar flash de 4 MB.
- [x] Inicializar Git.
- [x] Publicar no GitHub.

### Fase 1 — Fundamentos elétricos e GPIO

- níveis lógicos;
- alimentação e referência;
- entrada e saída;
- pull-up e pull-down;
- botões e debounce;
- interrupções;
- geração de pulsos;
- medição com analisador lógico.

### Fase 2 — UART

- transmissão e recepção;
- configuração de quadro;
- buffer circular;
- fila de eventos;
- perda de dados;
- captura;
- análise de baud rate;
- timestamps;
- logs.

### Fase 3 — I²C

- dispositivo mestre;
- leitura de sensores;
- scanner de endereços;
- ACK e NACK;
- pull-ups;
- falhas de barramento;
- captura e interpretação.

### Fase 4 — SPI

- configuração de mestre;
- modos SPI;
- múltiplos escravos;
- comunicação com microSD ou periférico;
- análise de clock e chip select;
- captura de transações.

### Fase 5 — RS-485 e Modbus RTU

- transceptor diferencial;
- controle half-duplex;
- construção e validação de quadros;
- CRC;
- timeouts;
- simulação de erros;
- diagnóstico de rede.

### Fase 6 — CAN / TWAI

- controlador TWAI;
- transceptor externo;
- filtros;
- mensagens;
- arbitragem;
- erros;
- estados do nó;
- análise do barramento.

### Fase 7 — Aquisição e diagnóstico

- módulo de captura;
- timestamps;
- buffers;
- filtros;
- contadores de erro;
- métricas;
- detecção de condições anormais;
- armazenamento de eventos.

### Fase 8 — Armazenamento

- microSD;
- arquivos CSV ou binários;
- sessões;
- integridade dos registros;
- recuperação após falhas;
- identificação de testes.

### Fase 9 — Interface embarcada simples

- servidor HTTP no ESP32;
- página HTML básica;
- seleção de interface;
- visualização de estado;
- visualização de contadores;
- exportação simples de registros.

### Fase 10 — Integração

- execução de múltiplos módulos;
- FreeRTOS;
- filas;
- eventos;
- prioridades;
- watchdog;
- monitoramento de heap;
- análise de desempenho;
- testes de longa duração.

### Fase 11 — Avaliação final

- revisão da arquitetura;
- revisão do firmware;
- repetição dos testes;
- análise das evidências;
- avaliação de autonomia;
- relatório de evolução;
- comparação com competências esperadas de um desenvolvedor júnior de firmware.

---

## Arquitetura planejada

A arquitetura deverá evoluir para módulos independentes:

```text
                       +----------------------+
Sinais externos ------>| Camada de aquisição |
                       +----------+-----------+
                                  |
                                  v
                       +----------------------+
                       | Buffers e timestamps |
                       +----------+-----------+
                                  |
                                  v
                       +----------------------+
                       | Decodificação e      |
                       | validação            |
                       +----------+-----------+
                                  |
                    +-------------+-------------+
                    |                           |
                    v                           v
          +-------------------+       +-------------------+
          | Diagnóstico       |       | Armazenamento     |
          | e métricas        |       | de evidências     |
          +---------+---------+       +---------+---------+
                    |                           |
                    +-------------+-------------+
                                  |
                                  v
                       +----------------------+
                       | Interface de usuário |
                       | simples              |
                       +----------------------+
```

A arquitetura real será definida gradualmente, com base nos testes e limitações observadas.

---

## Princípios de firmware

O código deverá seguir os seguintes princípios:

- módulos com responsabilidades claras;
- interfaces definidas em arquivos `.h`;
- implementação em arquivos `.c`;
- redução de dependências entre componentes;
- tratamento explícito de erros;
- logs com níveis adequados;
- uso consciente de memória;
- ausência de esperas ocupadas quando houver alternativa;
- uso de `vTaskDelay()` para temporização de tarefas;
- filas e eventos para comunicação entre tarefas;
- documentação de pinos e configurações;
- constantes no lugar de números mágicos;
- commits pequenos e rastreáveis;
- testes antes da integração.

---

## Estado atual

O primeiro firmware foi compilado e executado com sucesso em hardware real.

Validações realizadas:

- ESP32-D0WD-V3 detectado;
- revisão de silício v3.1;
- flash configurada para 4 MB;
- bootloader executado;
- tabela de partições carregada;
- aplicação carregada em `0x10000`;
- `app_main()` executada;
- ESP-IDF v6.0.1 confirmado;
- heap livre inicial de aproximadamente 303 kB;
- log periódico validado a cada dois segundos;
- versão do firmware associada ao commit Git;
- gravação e monitor serial realizados pelo WSL.

---

## Compilação

Ative o ESP-IDF:

```bash
source ~/esp/esp-idf/export.sh
```

Entre no projeto:

```bash
cd ~/projetos/ambiente_analisador
```

Defina o alvo, quando necessário:

```bash
idf.py set-target esp32
```

Compile:

```bash
idf.py build
```

---

## Gravação e monitor serial

Com o ESP32 conectado ao WSL:

```bash
idf.py -p /dev/ttyUSB0 flash monitor
```

Para sair do monitor:

```text
Ctrl + ]
```

A porta pode variar entre `/dev/ttyUSB0`, `/dev/ttyUSB1` ou `/dev/ttyACM0`.

---

## Fluxo de trabalho com Git

Verificar alterações:

```bash
git status
```

Adicionar arquivos:

```bash
git add .
```

Criar commit:

```bash
git commit -m "Describe the implemented stage"
```

Enviar ao GitHub:

```bash
git push
```

Cada commit deve representar um marco compreensível e validado.

---

## Evidências

As evidências podem incluir:

- logs de compilação;
- logs de gravação;
- saídas do monitor serial;
- fotografias das montagens;
- capturas do analisador lógico;
- medições de tensão;
- diagramas;
- tabelas de teste;
- resultados esperados e observados;
- registros de falhas;
- versões do firmware.

Os arquivos devem ser organizados por dia ou por etapa.

Exemplo:

```text
docs/06_evidencias/
├── dia_001/
│   └── flash_monitor_4mb.txt
├── uart/
├── i2c/
├── spi/
└── rs485/
```

---

## Critério de conclusão de uma etapa

Uma etapa será considerada concluída somente quando:

- [ ] o objetivo estiver documentado;
- [ ] a teoria essencial tiver sido estudada;
- [ ] o circuito estiver documentado;
- [ ] o firmware compilar sem erros;
- [ ] o teste em hardware tiver sido executado;
- [ ] o resultado esperado tiver sido comparado ao observado;
- [ ] houver evidência salva;
- [ ] as limitações forem registradas;
- [ ] o diário estiver atualizado;
- [ ] o código estiver versionado.

---

## Segurança e boas práticas

- Não conectar sinais sem verificar os níveis de tensão.
- O ESP32 trabalha com lógica de 3,3 V.
- Não aplicar 5 V diretamente a GPIOs do ESP32.
- Utilizar GND comum quando a interface exigir referência comum.
- Verificar a pinagem antes de energizar.
- Desligar o circuito antes de alterar conexões.
- Usar resistores de limitação em LEDs.
- Confirmar a alimentação de cada módulo.
- Não conectar o analisador lógico a tensões acima da especificação.
- Documentar qualquer adaptação de nível lógico.
- Evitar testes em equipamentos críticos ou desconhecidos.
- Realizar análises apenas em hardware próprio ou autorizado.

---

## Resultado esperado

Ao final, o projeto deverá demonstrar capacidade para:

- configurar e utilizar periféricos do ESP32;
- desenvolver firmware modular em ESP-IDF;
- compreender diferentes barramentos;
- analisar sinais digitais;
- identificar erros de comunicação;
- utilizar FreeRTOS de forma consciente;
- registrar e interpretar evidências;
- projetar testes;
- diagnosticar problemas de hardware e firmware;
- documentar decisões técnicas;
- trabalhar com versionamento;
- apresentar evolução consistente em engenharia de firmware.

---

## Licença

A licença ainda será definida. Até essa definição, o conteúdo deve ser considerado material educacional do autor.

---

## Autor

**Guilherme Costa**

Projeto iniciado em **10 de julho de 2026**.
