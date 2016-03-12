# DRT - DR Toolkit

## What is DR Toolkit

### What makes it different

DRT differs from other frameworks in process of the application development. While most of the frameworks require either lot of manual coding or declarative annotations, DRT allows to define most of the things in UML model and reuse it over all the model architecture. Most of the information are taken from the high-level business types and even software architect has a lot simpler job.

As a result it allows almost complete automation from analyzing the architecture to the application deployment. This approach minimizes the number of mistakes on architecture side, design and the development itself.

In general, DRT tries to avoid complexity where it is not necessary while adds complexity where it leads to cleaner and safer code and reusability of components.

### Development Targets

DRT consists of several parts, includes C++ framework, Java components, Php server-side and JavaScript client-side components.

DRT for Java is set of Java components, UML generators, and other tools for effective and automatic server-side development.

DRT for C++ is set of C++ libraries and other utilities for various development targets. Offers easy SQL integration, Web Services integration and many others. This part includes object framework, thread management, IO framework, network framework, and other application support classes.

DRT for Php part includes support for meta class management, server-side communication, validators, partial UML integration.

DRT for web client part includes client-side JavaScript support for meta class management, UML integration and server data communication.

### Design

DRT design is based on strict and clean object model.

### Platforms

DRT (C++) can be compiled for any popular architectures, including x86, x86_64, ARM and MIPS. In case of demand the set can be extended.

DRT can be run on UNIX systems, Linux, BSD, Windows system and also special mobile platforms such as Symbian, Windows Mobile, etc.

The biggest advantage is that DRT makes the target platform completely transparent for other applications.

## Development

### Object model

DRT is based on strict object model. The object design allows developer to almost not care about the lifetime of application components.

### Reusable components

DRT provides implementation of complete components. These are reusable and can be used without additional mappings, annotations, inheritance or any other kind of coding. This allows to build applications with descriptive commands rather than lot of source code.

### Web Inversion of Control

In case of web applications DRT implements Inversion of Control where parent object is responsible what happens with child path.

The architecture of web page is based on hierarchical tree of reusable components. The parent object creates (or asks for) component which it passes full control to. This allows full reusability of both parent object and child component without any need of additional coding. That is including all of model part, view part and controller part of common pattern. Security rules are mandatory part of relations definition.

### Exceptions

DRT strictly uses exceptions in case of unexpected situations. This allows developer to concentrate on application flow instead of tainting the code with error checks.

### UML integration

DRT allows automatic processing of UML diagrams and can generate code for various components, such as Entity Beans, Dao JPA objects, Dao Hibernate objects, WS services, Web forms, Validation classes, SQL tables, etc. It is very easy to implement complex RIA Ajax and REST based applications without writing a single line of code.

### Validation and Formatting

DRT implements annotation based validation, which can be again taken from UML diagrams based on business object definitions. This allows automatic processing and validation code to be specified in one place and be reusable through the whole architecture model. The same applies for formatting which can be specified in pair with validation rules.

Validation rules are applied transparently whenever the client sends the data to the server, transparently on both sides - client and server. Again, the aim is to specify the rules only once.

### ACL implementation

DRT implements ACL in DAO objects natively and transparently. ACL requirements are taken from object meta data or UML diagram and can be defined per class, per attribute for CRUD operations or object method. The access is inherited through class relationships such as composition, aggregation, etc.

## Download

### DR - core

<a href="../drt/">drt</a>

### DR - javascript part

<a href="../drjs/">drjs</a>

### DR - java part

<a href="../drjava/">drjava</a>

### DR - php part

<a href="../drphp/">drphp</a>
