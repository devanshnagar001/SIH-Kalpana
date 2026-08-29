# SIH26172 - Frozen Interface Contract



Project: Low Latency and Efficient Voice Activator for Edge Devices

Organisation: ISRO

Keyword: Kalpana

Hardware: ESP32-S3 + INMP441

Status: FROZEN

Contract version: v1.0

Last updated: 29 August 2026



This document defines the interfaces between the six project components.



These values are frozen for integration. Individual team members must not

silently change them. Any proposed change must be raised with P1, approved,

documented, and versioned.



## 1. Audio Input



| Parameter | Frozen value |

|---|---|

| Microphone | INMP441 |

| Sample rate | 16 kHz |

| Sample format | 16-bit signed |

| Channels | Mono |

| I2S slot | 32-bit |

| I2S conversion | Shift right by 11 |



Owner: P2 - Audio Capture



## 2. Audio Buffer



| Parameter | Frozen value |

|---|---|

| Ring buffer | 1.5-2 seconds |

| Pre-roll | 500 ms |



The buffer provides continuous audio to downstream processing and retains

500 ms of audio before keyword detection.



Owner: P2 - Audio Capture



## 3. Feature Extraction



| Parameter | Frozen value |

|---|---|

| Analysis window | 30 ms |

| Hop | 20 ms |

| Frames per 1-second window | 49 |

| MFCC coefficients | 10 |



Owner: P3 - Feature Parity



Python and ESP32 C implementations must produce matching features within

the agreed tolerance.



## 4. ML Model



| Parameter | Frozen value |

|---|---|

| Input tensor | INT8 [49][10] |

| Output classes | 3 |

| Classes | keyword / unknown / silence |

| Model size ceiling | <64 KB |

| Quantisation | INT8 |



Owner: P4 - Model



## 5. Detection



| Parameter | Frozen value |

|---|---|

| Detection window | Sliding 1 second |

| Detection stride | 200 ms |

| Smoothing | 3 consecutive posteriors |



Keyword detection must happen locally on the ESP32.



## 6. Network Streaming



| Parameter | Frozen value |

|---|---|

| Protocol | TCP |

| Audio format | Raw PCM |

| Sample rate | 16 kHz |

| Sample format | 16-bit signed mono |

| Framing | 0xA5A5 + length |



Owner: P5 - Network and ASR



## 7. ASR



ASR runs offline on the laptop using Vosk.



No cloud ASR is permitted.



## 8. System Budgets



| Metric | Requirement |

|---|---|

| RAM | <256 KB |

| Idle CPU | <10% |

| Model size | <64 KB |



## 9. Change Control



A frozen-contract change follows:



1. Propose the change.

2. Explain why it is necessary.

3. Check which modules are affected.

4. P1 approves or rejects the change.

5. Update this document.

6. Increment the contract version.

7. Announce the change to the team.



No team member should silently change an interface value.



## 10. Week 1 Contract Ownership



P1 maintains the frozen contract during integration.



For Week 1, the contract is considered frozen once this README is committed

to the shared repository and the freeze is announced to the team.



Any disagreement with a frozen value must be raised with P1 rather than

changed directly in code or documentation.





