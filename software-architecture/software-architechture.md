2022.11.03
---

# what is software architecture

Some keywords

- fundamental organization

- relationships

- all about software design

- focus on issues that difficult to change once the system is built: QA, non-functional requirements

- about structures but not just structures

- components/elements

- a set of principal design decisions

  - architecture = {principal design decisions}

- architecture is **not** a phase of development

- necessary for construction and evolution

## principle design decisions

- aspects: static(interface or something), dynamic

# what is software architect

- is a role not a rank

- make architectural decisions

- continually analyze the architecture

  - strive for consistency

  - organize the code into packages, folder, modules

- keep current with existing trends

  - be aware of the latest technology and industry trends

- be familiar with varying technologies

- teamwork and leadership skills

# what is stakeholders

- (explicitly or implicitly) drive the whole shape and direction of the architecture to serve their needs

# laws of software architecture

- everything in software architecture is a trade-off

- why is more impotent than how

# from architecture drivers(input) to architecture(output)

- drivers: design objectives, functional requirements, **quality attributes**, constraints, concerns

- Non-functional properties

  NFPS are the result of architecture choices

- Quality attributes(=NFP)

  - determine most architectural design decisions

# Quality attributes

- classify:

  - external, internal

  - static, dynamic

  - **runtime, non-runtime**

- it's a trade-off

- some important QAs

  - availability: time possible (201 seconds down/week)

  - reliability: attempts(1 time/1000 attempts)

  - manageability: manage to maintain health(one metric: time of per upgrade consumed)

  - flexibility: change architecture in a cost-effective manner(one metric: independence of codes)

  - performance

  - capacity: run a certain number of jobs per unit time

  -  scalability: the ability to support increasing services

    - vertical: come from adding capacity

    - horizontal: come from adding servers

  - extensibility: modify or add functionality without impacting the existing functionalist(code independence)

  - testability: can determine the results 

- twin peaks model

# Why documents

- contributes to architecture design

- clarifies the line between architecture and non-architecture design decisions

- **three uses**: education(to newcomer), communication(among stakeholders), analysis

- goal:

  - main goal: communicate the structure

  - **common vocabulary**

  - describe the software (what and how)

- rules: 

  - avoid unnecessary repetition

  - avoid ambiguity

  - use standard organization or template

  - keep current

# Views and viewpoints

- view: the **consequence**, representation of system

- viewpoint: how to structure the system, the **process**

- number of views depends on stakeholders' needs

# C4 model

- Context, container, components, code

- Container is responsible for components. 

- Container can block the user requests.(AOP)

- `+` connector = C5

# arch42

- from the aspects of problem and solution

- block view: component diagram

- runtime view: sequence diagram

- deployment view: deployment diagram

- architectural decisions: describe why choose this(rationale)

- quality requirements: stimulus vs response

# how to choose QAs

- tech: runtime, non-runtime

- non-tech: business

# 4+1 views

- logical view: for end user, how to use

  - similar to c4 component view
  
  - functional requirements

  - diagram:
  
    - class diagram or component diagram: logical relationships
    
    - state transition diagrams

    - data driven: ER diagrams

  - map use cases onto components

- process view: for system integrator, how to manage, the whole process

  - dynamic aspects of the architecture

  - not all system need this view(such as simple systems)

  - performance, behavior, communication, concurrency

  - this process is not the same as the OS process
  
  - divide into independent tasks:

    - (why?)to distinguish the major tasks(communicate through network) and the minor tasks(through memory)

  - sequence diagram, component diagram

- implementation view: for software management(programmer), how to code

  - static organization of software code

  - map logical view onto code

  - module organization, 'export' and 'import' relationships

- deployment view(physical view): for system engineer

  - non-functional requirements

  - availability, reliability

  - mapping logical to physical entities

- (scenario)use case: the one which deduce other 4 views

  - an abstraction of the most important requirements

  - useful in all circumstance

- 4 views are not fully orthogonal or independent

