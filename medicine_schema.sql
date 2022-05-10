-- Table: public.medicines

-- DROP TABLE IF EXISTS public.medicines;

CREATE TABLE IF NOT EXISTS public.medicines
(
    id integer NOT NULL DEFAULT nextval('medicines_id_seq'::regclass),
    name character varying(32) COLLATE pg_catalog."default" NOT NULL,
    owned_by character varying(32) COLLATE pg_catalog."default" NOT NULL,
    arrival_date character varying(16) COLLATE pg_catalog."default" NOT NULL,
    expire_date character varying(16) COLLATE pg_catalog."default" NOT NULL,
    price double precision NOT NULL DEFAULT 0.00,
    qty integer NOT NULL DEFAULT 0,
    CONSTRAINT medicines_pkey PRIMARY KEY (id)
)

TABLESPACE pg_default;

ALTER TABLE IF EXISTS public.medicines
    OWNER to rob;
